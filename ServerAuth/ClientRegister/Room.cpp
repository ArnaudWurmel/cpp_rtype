//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "Room.hh"

unsigned int    rtp::Room::_roomId = 0;

rtp::Room::Room(unsigned int ownerId, std::shared_ptr<rtp::IServerRegister>& iServerRegister) : _iServerRegister(iServerRegister) {
    _id = rtp::Room::_roomId++;
    _ownerId = ownerId;
    _isOpen = true;
    _onMatchmaking = false;
    _haveAServer = false;
}

unsigned int    rtp::Room::getId() const {
    return _id;
}

bool    rtp::Room::addPlayer(std::shared_ptr<RegisteredClient>& player) {
    std::unique_lock<std::mutex>    lck(_locker);

    if (_playerList.size() >= 4 || _onMatchmaking) {
        return false;
    }
    auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&] (std::shared_ptr<RegisteredClient> const& playerCmp) {
        return playerCmp->getId() == player->getId();
    });
    if (iterator != _playerList.end()) {
        return false;
    }
    _playerList.push_back(player);
    return true;
}

bool    rtp::Room::isOpen() const {
    return _isOpen;
}

bool    rtp::Room::removePlayer(std::shared_ptr<RegisteredClient>& player) {
    std::unique_lock<std::mutex>    lck(_locker);

    auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&] (std::shared_ptr<RegisteredClient> const& playerCmp) {
        return playerCmp->getId() == player->getId();
    });
    if (iterator == _playerList.end()) {
        return false;
    }
    bool    isOwner = player->getId() == _ownerId;
    _playerList.erase(iterator);
    if (isOwner && _playerList.size() > 0) {
        NetworkAbstract::Message    newOwnerMessage;
        auto newOwner = (_playerList.begin() + (rand() % (_playerList.size() - 1)));

        newOwnerMessage.setType(RegisteredClient::Command::NewOwner);
        (*newOwner)->write(newOwnerMessage);
    }
    else {
        _isOpen = false;
    }
    return true;
}

bool    rtp::Room::isPlayerIn(std::shared_ptr<RegisteredClient>& player) {
    std::unique_lock<std::mutex>    lck(_locker);
    auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&] (std::shared_ptr<RegisteredClient> const& playerCmp) {
        return playerCmp->getId() == player->getId();
    });
    return iterator != _playerList.end();
}

bool    rtp::Room::findAServer(std::shared_ptr<RegisteredClient>& player) {
    if (player->getId() != _ownerId || _onMatchmaking || _haveAServer) {
        return false;
    }
    auto    it = _playerList.begin();
    NetworkAbstract::Message    message;

    message.setType(RegisteredClient::StartMatchmaking);
    while (it != _playerList.end()) {
        (*it)->write(message);
        ++it;
    }

    _onMatchmaking = true;
    _matchmakingFinder = std::unique_ptr<std::thread>(new std::thread([&] {
        while (_onMatchmaking) {
            std::unique_lock<std::mutex>    lck(_locker);
            _iServerRegister->lockData();
            auto iterator = _iServerRegister->getServer().begin();

            while (iterator != _iServerRegister->getServer().end()) {
                if ((*iterator)->getState() == RegisteredServer::ServerState::Available) {
                    NetworkAbstract::Message    message;
                    std::string bodyContent;

                    message.setType(RegisteredServer::Command::RESERVED);
                    (*iterator)->write(message);
                    message.setType(RegisteredClient::Command::ServerFound);

                    bodyContent = (*iterator)->getIpAddr() + ":" + std::to_string((*iterator)->getPort()) + ";" + (*iterator)->getRegistrationToken();
                    message.setBody(bodyContent.c_str(), bodyContent.length());
                    auto iteratorPlayer = _playerList.begin();

                    while (iteratorPlayer != _playerList.end()) {
                        (*iteratorPlayer)->write(message);
                        ++iteratorPlayer;
                    }
                    _haveAServer = true;
                    _onMatchmaking = false;
                    _iServerRegister->unlockData();
                    return ;
                }
                ++iterator;
            }
            _iServerRegister->unlockData();
        }
    }));
    return true;
}

bool    rtp::Room::stopMatchmaking(std::shared_ptr<RegisteredClient>& player) {
    std::unique_lock<std::mutex>    lck(_locker);
    if (player->getId() != _ownerId) {
        return false;
    }
    _iServerRegister->lockData();
    auto    it = _playerList.begin();
    NetworkAbstract::Message    message;

    message.setType(RegisteredClient::StopMatchmaking);
    while (it != _playerList.end()) {
        (*it)->write(message);
        ++it;
    }
    _onMatchmaking = false;
    _haveAServer = false;
    _iServerRegister->unlockData();
    _matchmakingFinder->join();
    _matchmakingFinder.reset();
    return true;
}

unsigned long    rtp::Room::nbPlayerIn() const {
    return _playerList.size();
}

std::vector<std::shared_ptr<rtp::RegisteredClient> > const& rtp::Room::getPlayerList() const {
    return _playerList;
}

rtp::Room::~Room() {
    if (_onMatchmaking && _matchmakingFinder) {
        _onMatchmaking = false;
        _matchmakingFinder->join();
    }
}