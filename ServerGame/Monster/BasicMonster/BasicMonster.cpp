//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "BasicMonster.hh"
#include "Entities/Vector2.hh"

Monster::BasicMonster::BasicMonster(unsigned int id, int x, int y) : rtp::AEnemy(id, "BasicMonster" + std::to_string(rand() % 4) + ".png", x, y, 1) {
    addCollideRect(rtp::CollideRect(0, 0, 61, 41));
    addCollideRect(rtp::CollideRect(62, 0, 62, 39));
    addCollideRect(rtp::CollideRect(128, 0, 54, 39));
    addCollideRect(rtp::CollideRect(62, 0, 62, 39));
    addCollideRect(rtp::CollideRect(0, 0, 61, 41));
    _frame = 0;
}

void    Monster::BasicMonster::update(double diff) {
    translate(rtp::Vector2<int> {0, (int)(200 * diff)});
    setUpdated(true);
    if (_frame % 10 == 0) {
        _currentFrame = (_currentFrame + 1) % _collideRectList.size();
        _frame = 0;
    }
    ++_frame;
    if (getCollideRect().getY() > HEIGHT) {
        this->hit(_lifePoint);
    }
}

Monster::BasicMonster::~BasicMonster() = default;