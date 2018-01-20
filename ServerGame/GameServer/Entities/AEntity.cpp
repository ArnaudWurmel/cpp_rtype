//
// Created by Arnaud WURMEL on 15/01/2018.
//

#include "AEntity.hh"
#include "APlayer.hh"

unsigned int    rtp::AEntity::_entityIdIncr = 0;

unsigned int    rtp::AEntity::getNextId() {
    return _entityIdIncr++;
}

rtp::AEntity::AEntity(std::string const& spriteName, int x, int y) {
    _spriteName = spriteName;
    _currentFrame = 0;
    _position.x = x;
    _position.y = y;
    _updated = false;
    _entityId = _entityIdIncr++;
    _rotation = 0;
}

rtp::AEntity::AEntity(unsigned int entityId, std::string const& spriteName, int x, int y) {
    _spriteName = spriteName;
    _currentFrame = 0;
    _position.x = x;
    _position.y = y;
    _updated = false;
    _entityId = entityId;
    _rotation = 0;
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

    info = std::to_string(_entityId) + " " + std::to_string(_rotation) + " " + std::to_string(_position.x) + " " + std::to_string(_position.y) + " " + std::to_string(_currentFrame);
    return info;
}

std::string rtp::AEntity::getInfoProtected() const {
    std::string info;

    info = std::to_string(_position.x) + " " + std::to_string(_position.y) + " " + std::to_string(_currentFrame);
    return info;
}

std::string&    rtp::AEntity::operator>>(std::string& dest) const {
    dest = dest + getInfos() + " " + _spriteName + " ";
    std::string content;
    auto iterator = _collideRectList.begin();
    while (iterator != _collideRectList.end()) {
        content = content + std::to_string((*iterator).getX()) + "." + std::to_string((*iterator).getY()) + "." + std::to_string((*iterator).getWidth()) + "." + std::to_string((*iterator).getHeight());
        ++iterator;
        if (iterator != _collideRectList.end()) {
            content += ";";
        }
    }
    dest = dest + content;
    return dest;
}

unsigned int    rtp::AEntity::getEntityId() const {
    return _entityId;
}

bool    rtp::AEntity::isExpectedToBeDeleted() const {
    return false;
}

rtp::AEntity::~AEntity() = default;