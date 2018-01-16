//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "AEntity.hh"

rtp::AEntity::AEntity(std::string const& spriteName) {
    _spriteName = spriteName;
    _currentFrame = 0;
    _position.x = 500;
    _position.y = 500;
    _updated = false;
}

rtp::CollideRect rtp::AEntity::getCollideRect() const {
    rtp::CollideRect    rect = _collideRectList[_currentFrame];

    rect.translate(_position);
    return rect;
}

bool    rtp::AEntity::collide(AEntity const& entity) {
    return false;
}

void    rtp::AEntity::translate(Vector2<int> const& translateValue) {
    _position.x += translateValue.x;
    _position.y += translateValue.y;
    _updated = true;
}

void    rtp::AEntity::addCollideRect(CollideRect const& rect) {
    _collideRectList.push_back(rect);
}

bool    rtp::AEntity::isUpdated() const {
    return _updated;
}

void    rtp::AEntity::setUpdated(bool updated) {
    _updated = updated;
}

rtp::AEntity::~AEntity() = default;