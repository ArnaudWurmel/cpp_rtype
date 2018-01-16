//
// Created by Arnaud WURMEL on 14/01/2018.
//

#include <iostream>
#include "APlayer.hh"
#include "../../NetworkAbstract/Message/Message.h"

unsigned int    rtp::APlayer::_clientIdIncr = 0;

rtp::APlayer::APlayer(OnAuthorization const& authorizationCallback, std::string const& authToken) : AEntity("ShipSprite.png") {
    _id = _clientIdIncr++;
    _authorized = false;
    _authorizationCallback = authorizationCallback;
    _authToken = authToken;
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::AUTHORIZE, std::bind(&rtp::APlayer::handleAuthorize, this, std::placeholders::_1)));
    _collideRectList.push_back(CollideRect(0, 0, 24, 42));
    _collideRectList.push_back(CollideRect(43, 0, 35, 42));
    _collideRectList.push_back(CollideRect(86, 0, 49, 42));
    _collideRectList.push_back(CollideRect(141, 0, 35, 42));
    _collideRectList.push_back(CollideRect(195, 0, 23, 42));
    _currentFrame = Center;
}

bool    rtp::APlayer::isAuthorized() const {
    return _authorized;
}

bool    rtp::APlayer::injectInput(NetworkAbstract::Message const& message) {
    if (_functionPtrs.find(NetworkAbstract::getTypeOf<Command>(message)) != _functionPtrs.end()) {
        return _functionPtrs[NetworkAbstract::getTypeOf<Command>(message)](message);
    }
    return false;
}

bool    rtp::APlayer::handleAuthorize(NetworkAbstract::Message const& message) {
    if (message.getBodySize() == 0) {
        return false;
    }
    std::string input(message.getBody(), message.getBodySize());
    std::vector<std::string>    authToken = rtp::APlayer::getTokenFrom(input, ' ');
    _pseudo = authToken[0];
    _authorized = authToken[1].compare(_authToken) == 0;
    if (_authorized) {
        std::cout << "Player authorized to play" << std::endl;
    }
    _authorizationCallback(_authorized);
    return _authorized;
}

std::string const&  rtp::APlayer::getPseudo() const {
    return _pseudo;
}

unsigned int    rtp::APlayer::getId() const {
    return _id;
}

std::vector<std::string>    rtp::APlayer::getTokenFrom(std::string const& input, char sep) {
    std::vector<std::string>    tokenList;
    std::string::const_iterator   it;

    it = input.begin();
    while (it != input.end()) {
        std::string token;
        while (it != input.end() && *it != ' ') {
            token += *it;
            ++it;
        }
        if (token.size())
            tokenList.push_back(token);
        while (it != input.end() && *it == ' ')
            ++it;
    }
    return tokenList;
}

std::string&    rtp::APlayer::operator>>(std::string& dest) const {
    std::string content;

    content = std::to_string(_id) + " " + _pseudo + " " + _spriteName + " " + std::to_string(_position.x) + " " + std::to_string(_position.y) + " " + std::to_string(_currentFrame) + " ";
    auto iterator = _collideRectList.begin();
    while (iterator != _collideRectList.end()) {
        content = content + std::to_string((*iterator).getX()) + "." + std::to_string((*iterator).getY()) + "." + std::to_string((*iterator).getWidth()) + "." + std::to_string((*iterator).getHeight());
        ++iterator;
        if (iterator != _collideRectList.end()) {
            content += ";";
        }
    }
    dest += content;
    return dest;
}

rtp::APlayer::~APlayer() {}