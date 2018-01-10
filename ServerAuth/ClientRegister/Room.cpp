//
// Created by Arnaud WURMEL on 10/01/2018.
//

#include "Room.hh"

unsigned int    rtp::Room::_roomId = 0;

rtp::Room::Room(unsigned int ownerId) {
    _id = rtp::Room::_roomId++;
    _ownerId = ownerId;
}

unsigned int    rtp::Room::getId() const {
    return _id;
}

bool    rtp::Room::addPlayer(std::shared_ptr<RegisteredClient>& player) {
    if (_playerList.size() >= 4) {
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

bool    rtp::Room::removePlayer(std::shared_ptr<RegisteredClient>& player) {
    auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&] (std::shared_ptr<RegisteredClient> const& playerCmp) {
        return playerCmp->getId() == player->getId();
    });
    if (iterator == _playerList.end()) {
        return false;
    }
    _playerList.erase(iterator);
    return true;
}

bool    rtp::Room::isPlayerIn(std::shared_ptr<RegisteredClient>& player) {
    auto iterator = std::find_if(_playerList.begin(), _playerList.end(), [&] (std::shared_ptr<RegisteredClient> const& playerCmp) {
        return playerCmp->getId() == player->getId();
    });
    return iterator != _playerList.end();
}

unsigned long    rtp::Room::nbPlayerIn() const {
    return _playerList.size();
}

rtp::Room::~Room() {}