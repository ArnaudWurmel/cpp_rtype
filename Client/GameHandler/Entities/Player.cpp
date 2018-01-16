//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include <cstring>
#include <string>
#include "../../DataGetter/DataGetter.hh"
#include "Player.hh"
#include "../../Exceptions/Exception.hh"

std::shared_ptr<rtp::Player>    rtp::Player::instanciateFromInfo(std::string const& pInfo) {
    std::vector<std::string>    tokenList = DataGetter::getTokenFrom(pInfo, ' ');

    std::cout << pInfo << std::endl;
    if (tokenList.size() != 7) {
        throw ParsingNetworkException();
    }
    std::shared_ptr<rtp::Player>    player(new Player(std::stoi(tokenList[0]), tokenList[1], tokenList[2], std::stoi(tokenList[3]), std::stoi(tokenList[4]), std::stoi(tokenList[5])));
    player->parseFrame(DataGetter::getTokenFrom(tokenList[6], ';'));
    return  player;
}

rtp::Player::Player(int id, std::string const& pseudo, std::string const& spriteName, int x, int y, int currentFrame) : ADrawableEntity(spriteName, currentFrame, x, y, pseudo) {
    _id = id;
}

int rtp::Player::getId() const {
    return _id;
}

rtp::Player::~Player() {}