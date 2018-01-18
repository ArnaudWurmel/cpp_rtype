//
// Created by Arnaud WURMEL on 14/01/2018.
//

#include <iostream>
#include "APlayer.hh"
#include "../NetworkAbstract/Message/Message.h"

rtp::APlayer::APlayer() {
    _authorized = false;
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

void    rtp::APlayer::setAuthorize(bool authorize) {
    _authorized = authorize;
}

rtp::APlayer::~APlayer() {}