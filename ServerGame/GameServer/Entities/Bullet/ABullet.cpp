//
// Created by Arnaud WURMEL on 19/01/2018.
//

#include "ABullet.hh"

std::string rtp::ABullet::getSpriteFromBulletType(BulletType type) {
    switch (type) {
        case Ally:
            return "AllyBullet.png";
        case Monster:
            return "EnemyBullet.png";
        default:
            return "";
    }
}

rtp::ABullet::ABullet(BulletType type, Vector2<int> position, Vector2<int> direction) : AEntity(getSpriteFromBulletType(type), position.x, position.y), _direction(direction), _type(type) {
    _alive = true;
    _collideRectList.push_back(CollideRect(0, 0, 9, 19));
    _collideRectList.push_back(CollideRect(11, 0, 9, 19));
    _collideRectList.push_back(CollideRect(22, 0, 10, 19));
    _collideRectList.push_back(CollideRect(34, 0, 9, 19));
    _collideRectList.push_back(CollideRect(45, 0, 9, 19));
    _currentFrame = 0;
}

void    rtp::ABullet::update(double diff) {
    setUpdated(true);
    translate(Vector2<int> {(int)(_direction.x * diff), (int)(_direction.y * diff)});
    if (_position.x < 0 || _position.x + getCollideRect().getWidth() >= WIDTH ||
                            _position.y < 0 || _position.y + getCollideRect().getHeight() >= HEIGHT) {
        _alive = false;
    }
}

bool    rtp::ABullet::isExpectedToBeDeleted() const {
    return !_alive;
}

rtp::ABullet::~ABullet() = default;