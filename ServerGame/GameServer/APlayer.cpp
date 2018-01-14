//
// Created by Arnaud WURMEL on 14/01/2018.
//

#include <iostream>
#include "APlayer.hh"
#include "../NetworkAbstract/Message/Message.h"

rtp::APlayer::APlayer(std::string const& authToken) {
    _authorized = false;
    _authToken = authToken;
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::AUTHORIZE, std::bind(&rtp::APlayer::handleAuthorize, this, std::placeholders::_1)));
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
    std::string token(message.getBody(), message.getBodySize());
    _authorized = token.compare(_authToken) == 0;
    if (_authorized) {
        std::cout << "Player authorized to play" << std::endl;
    }
    return _authorized;
}

rtp::APlayer::~APlayer() {}