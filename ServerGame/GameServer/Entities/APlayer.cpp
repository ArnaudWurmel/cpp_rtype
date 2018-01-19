//
// Created by Arnaud WURMEL on 14/01/2018.
//

#include <iostream>
#include "APlayer.hh"
#include "../../NetworkAbstract/Message/Message.h"

unsigned int    rtp::APlayer::_clientIdIncr = 0;

rtp::APlayer::APlayer() : AEntity("ShipSprite.png", 500, 500) {
    _id = _clientIdIncr++;
    _authorized = false;
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::FORWARD, std::bind(&rtp::APlayer::handleForward, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::BACKWARD, std::bind(&rtp::APlayer::handleBackward, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::LEFT, std::bind(&rtp::APlayer::handleLeft, this, std::placeholders::_1)));
    _functionPtrs.insert(std::make_pair(rtp::APlayer::Command::RIGHT, std::bind(&rtp::APlayer::handleRight, this, std::placeholders::_1)));
    _collideRectList.push_back(CollideRect(0, 0, 24, 42));
    _collideRectList.push_back(CollideRect(43, 0, 35, 42));
    _collideRectList.push_back(CollideRect(86, 0, 49, 42));
    _collideRectList.push_back(CollideRect(141, 0, 35, 42));
    _collideRectList.push_back(CollideRect(195, 0, 23, 42));
    _currentFrame = Center;
    _moveMapping.push_back(std::make_pair(false, std::bind(&rtp::APlayer::forward, this, std::placeholders::_1)));
    _moveMapping.push_back(std::make_pair(false, std::bind(&rtp::APlayer::backward, this, std::placeholders::_1)));
    _moveMapping.push_back(std::make_pair(false, std::bind(&rtp::APlayer::left, this, std::placeholders::_1)));
    _moveMapping.push_back(std::make_pair(false, std::bind(&rtp::APlayer::right, this, std::placeholders::_1)));
    _modifierFrameIncr = 0;
    _noUpdatedCount = 0;
}

bool    rtp::APlayer::isAuthorized() const {
    return _authorized;
}

void    rtp::APlayer::setAuthorize(bool value) {
    _authorized = value;
}

void    rtp::APlayer::setPseudo(std::string const& pseudo) {
    _pseudo = pseudo;
}

bool    rtp::APlayer::injectInput(NetworkAbstract::Message const& message) {
    if (_functionPtrs.find(NetworkAbstract::getTypeOf<Command>(message)) != _functionPtrs.end()) {
        return _functionPtrs[NetworkAbstract::getTypeOf<Command>(message)](message);
    }
    return false;
}

std::string const&  rtp::APlayer::getPseudo() const {
    return _pseudo;
}

unsigned int    rtp::APlayer::getId() const {
    return _id;
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

std::string rtp::APlayer::getInfos() const {
    std::string pInfo = std::to_string(_id) + " " + AEntity::getInfos();

    return pInfo;
}

bool    rtp::APlayer::handleForward(NetworkAbstract::Message const &) {
    _moveMapping[mapForward].first = true;
    std::cout << "Handle forward" << std::endl;
    return true;
}

bool    rtp::APlayer::handleBackward(NetworkAbstract::Message const &) {
    _moveMapping[mapBackward].first = true;
    return true;
}

bool    rtp::APlayer::handleLeft(NetworkAbstract::Message const &) {
    _moveMapping[mapLeft].first = true;
    return true;
}

bool    rtp::APlayer::handleRight(NetworkAbstract::Message const &) {
    _moveMapping[mapRight].first = true;
    return true;
}

void    rtp::APlayer::handleMoving(double diff) {
    auto iterator = _moveMapping.begin();

    std::cout << diff << std::endl;
    while (iterator != _moveMapping.end()) {
        if ((*iterator).first) {
            setUpdated(true);
            (*iterator).first = false;
            (*iterator).second(diff);
        }
        ++iterator;
    }
    if (!isUpdated() && _currentFrame != Center) {
        if (_noUpdatedCount >= 5) {
            if (_currentFrame < Center) {
                ++_currentFrame;
            }
            else if (_currentFrame > Center) {
                --_currentFrame;
            }
            setUpdated(true);
            _noUpdatedCount = 0;
        }
        else {
            ++_noUpdatedCount;
        }
    }
    else {
        _noUpdatedCount = 0;
    }
}

void    rtp::APlayer::forward(double diff) {
    translate(Vector2<int> {0, (int)(-300 * diff)});
    _currentFrame = Center;
}

void    rtp::APlayer::backward(double diff) {
    translate(Vector2<int> {0, (int)(300 * diff)});
    _currentFrame = Center;
}

void    rtp::APlayer::left(double diff) {
    translate(Vector2<int> {(int)(-300 * diff), 0});
    if (_currentFrame != Left && _currentFrame != FullLeft) {
        _modifierFrameIncr = 0;
    }
    if (_currentFrame == Left && _modifierFrameIncr >= 5) {
        _currentFrame = FullLeft;
        _modifierFrameIncr = 0;
    }
    else if (_currentFrame == Left) {
        ++_modifierFrameIncr;
    }
    else if (_currentFrame != FullLeft) {
        _currentFrame = Left;
    }
}

void    rtp::APlayer::right(double diff) {
    translate(Vector2<int> {(int)(300 * diff), 0});
    if (_currentFrame != Right && _currentFrame != FullRight) {
        _modifierFrameIncr = 0;
    }
    if (_currentFrame == Right && _modifierFrameIncr >= 5) {
        _currentFrame = FullRight;
        _modifierFrameIncr = 0;
    }
    else if (_currentFrame == Right) {
        ++_modifierFrameIncr;
    }
    else if (_currentFrame != FullRight) {
        _currentFrame = Right;
    }
}

void    rtp::APlayer::resetAnimation() {
    _currentFrame = Center;
}

rtp::APlayer::~APlayer() {
    std::cout << "Player deleted" << std::endl;
}