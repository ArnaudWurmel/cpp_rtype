//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <cstring>
#include <string>
#include "../../DataGetter/DataGetter.hh"
#include "Player.hh"
#include "../../Exceptions/Exception.hh"

std::shared_ptr<rtp::Player>    rtp::Player::instanciateFromInfo(std::string const& pInfo, bool me) {
    std::vector<std::string>    tokenList = DataGetter::getTokenFrom(pInfo, ' ');

    if (tokenList.size() != 7) {
        throw ParsingNetworkException();
    }
    std::shared_ptr<rtp::Player>    player(new Player(std::stoi(tokenList[0]), tokenList[1], tokenList[2], std::stoi(tokenList[3]), std::stoi(tokenList[4]), std::stoi(tokenList[5]), me));
    player->parseFrame(DataGetter::getTokenFrom(tokenList[6], ';'));
    return  player;
}

rtp::Player::Player(int id, std::string const& pseudo, std::string const& spriteName, int x, int y, int currentFrame, bool me) : ADrawableEntity(spriteName, currentFrame, x, y, pseudo) {
    _id = id;
    _me = me;
}

int rtp::Player::getId() const {
    return _id;
}

void    rtp::Player::updateFrom(std::vector<std::string> const& pInfos) {
    if (pInfos.size() == 4) {
        _renderLock.lock();
        _x = std::stoi(pInfos[1]);
        _y = std::stoi(pInfos[2]);
        _currentFrame = std::stoi(pInfos[3]);
        _renderLock.unlock();
    }
}

bool    rtp::Player::isMe() const {
    return _me;
}

rtp::Player::~Player() {}