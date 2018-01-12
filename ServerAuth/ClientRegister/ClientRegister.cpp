//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "ClientRegister.hh"

rtp::ClientRegister::ClientRegister(unsigned short port, std::shared_ptr<IServerRegister> iServerRegister) : BaseServer(port) {
    _iServerRegister = iServerRegister;
}

void    rtp::ClientRegister::serverLooping() {
    try {
        _acceptor->run();
    }
    catch (std::exception& e) {
        say(std::string("Exception thread running : ") + e.what());
        _threadRunning = false;
    }
    while (_threadRunning) {
        std::unique_lock<std::mutex>    lck(_mClient);
        while (_acceptor->haveAwaitingClient()) {
            std::shared_ptr<RegisteredClient>   client(new RegisteredClient(_acceptor->acceptClient(), *this));

            say("New client");
            NetworkAbstract::Message    message;
            client->write(message);
            _clientList.push_back(client);
        }
        auto iterator = _clientList.begin();

        while (iterator != _clientList.end()) {
            if (!(*iterator)->isOpen() || !(*iterator)->handleNewData()) {
                playerLeaveRoom(*iterator);
                say("Loose connection with client{" + std::to_string((*iterator)->getId()) + "}");
                _clientList.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
        auto iteratorRoom = _roomList.begin();

        while (iteratorRoom != _roomList.end()) {
            if (!(*iteratorRoom)->isOpen()) {
                _roomList.erase(iteratorRoom);
            }
            else {
                ++iteratorRoom;
            }
        }
    }
}

bool    rtp::ClientRegister::playerCreateRoom(std::shared_ptr<RegisteredClient> player) {
    if (getPlayerRoomId(player) != -1) {
        return false;
    }
    _roomList.push_back(std::unique_ptr<Room>(new Room(player->getId(), _iServerRegister)));
    _roomList.back()->addPlayer(player);
    return true;
}

bool    rtp::ClientRegister::playerLeaveRoom(std::shared_ptr<RegisteredClient> player) {
    if (getPlayerRoomId(player) == -1) {
        return false;
    }
    auto iterator = std::find_if(_roomList.begin(), _roomList.end(), [&](std::unique_ptr<Room> const& room) {
        return room->getId() == getPlayerRoomId(player);
    });
    bool success = (*iterator)->removePlayer(player);
    if ((*iterator)->nbPlayerIn() == 0) {
        _roomList.erase(iterator);
    }
    return success;
}

bool    rtp::ClientRegister::playerJoinRoom(std::shared_ptr<RegisteredClient> player, int roomId) {
    if (getPlayerRoomId(player) != -1) {
        return false;
    }
    auto iterator = std::find_if(_roomList.begin(), _roomList.end(), [&](std::unique_ptr<Room> const& room) {
        return room->getId() == roomId;
    });
    if (iterator == _roomList.end()) {
        return false;
    }
    return (*iterator)->addPlayer(player);
}

bool    rtp::ClientRegister::playerStartMatchmaking(std::shared_ptr<RegisteredClient> player) {
    if (getPlayerRoomId(player) == -1) {
        return false;
    }
    auto iterator = std::find_if(_roomList.begin(), _roomList.end(), [&](std::unique_ptr<Room> const& room) {
        return room->isPlayerIn(player);
    });
    if (iterator == _roomList.end()) {
        return false;
    }
    return (*iterator)->findAServer(player);
}

bool    rtp::ClientRegister::playerStopMatchmaking(std::shared_ptr<RegisteredClient> player) {
    if (getPlayerRoomId(player) == -1) {
        return false;
    }
    auto iterator = std::find_if(_roomList.begin(), _roomList.end(), [&](std::unique_ptr<Room> const& room) {
        return room->isPlayerIn(player);
    });
    if (iterator == _roomList.end()) {
        return false;
    }
    return (*iterator)->stopMatchmaking(player);
}

std::vector<std::unique_ptr<rtp::Room> > const&  rtp::ClientRegister::playerAskRoomList() const {
    return _roomList;
}

int rtp::ClientRegister::getPlayerRoomId(std::shared_ptr<RegisteredClient> player) const {
    auto iterator = std::find_if(_roomList.begin(), _roomList.end(), [&](std::unique_ptr<Room> const& room) {
        return room->isPlayerIn(player);
    });
    if (iterator == _roomList.end()) {
        return -1;
    }
    return (*iterator)->getId();
}

rtp::ClientRegister::~ClientRegister() = default;