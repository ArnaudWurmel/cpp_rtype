//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "AEntity.hh"

rtp::AEntity::AEntity(std::string const& spriteName, int nbFrame, CollideRect const& collideRect) : _collideRect(collideRect) {
    _spriteName = spriteName;
    _nbFrame = nbFrame;
    _currentFrame = 0;
}

rtp::CollideRect const& rtp::AEntity::getCollideRect() const {
    return _collideRect;
}

bool    rtp::AEntity::collide(AEntity const& entity) {
    return false;
}

void    rtp::AEntity::translate(Vector2<int> const& translateValue) {
    _collideRect.translate(translateValue);
}

rtp::AEntity::~AEntity() = default;