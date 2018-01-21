//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "BossMonster.hh"
#include "Entities/Vector2.hh"

Monster::BossMonster::BossMonster(unsigned int id, int x, int y) : rtp::AEnemy(id, "BossMonster.png", x, y, 50) {
    addCollideRect(rtp::CollideRect(0, 0, 200, 200));
    _frame = 0;
}

void    Monster::BossMonster::update(double diff) {
    setUpdated(true);
    _rotation = (_rotation + (diff * 360));
    _rotation %= 360;
}

Monster::BossMonster::~BossMonster() = default;