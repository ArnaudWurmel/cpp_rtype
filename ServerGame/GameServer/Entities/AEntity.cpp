//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "AEntity.hh"

rtp::AEntity::AEntity(std::string const& spriteName, int x, int y) {
    _spriteName = spriteName;
    _currentFrame = 0;
    _position.x = x;
    _position.y = y;
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

std::string rtp::AEntity::getInfos() const {
    std::string info;

    info = std::to_string(_position.x) + " " + std::to_string(_position.y) + " " + std::to_string(_currentFrame);
    return info;
}

rtp::AEntity::~AEntity() = default;