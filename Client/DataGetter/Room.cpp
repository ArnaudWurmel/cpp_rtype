//
// Created by Arnaud WURMEL on 12/01/2018.
//

#include "Room.hh"
#include "DataGetter.hh"

rtp::Room::Room() = default;

bool    rtp::Room::init(std::string const& input) {
    std::vector<std::string>    tokenList = DataGetter::getTokenFrom(input);

    if (tokenList.empty()) {
        return false;
    }
    _id = std::stoi(tokenList[0]);

    auto iterator = tokenList.begin() + 1;

    while (iterator != tokenList.end()) {
        _playerList.push_back(*iterator);
        ++iterator;
    }
    return true;
}

int rtp::Room::getId() const {
    return _id;
}

std::vector<std::string> const& rtp::Room::getPlayerList() const {
    return _playerList;
}

rtp::Room::~Room() = default;