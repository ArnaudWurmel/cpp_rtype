//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "BasicMonster.hh"

Monster::BasicMonster::BasicMonster(int x, int y) : rtp::AEnemy("BasicMonster.png", x, y, 2) {
    addCollideRect(rtp::CollideRect(0, 0, 61, 41));
    addCollideRect(rtp::CollideRect(62, 0, 62, 39));
    addCollideRect(rtp::CollideRect(128, 0, 54, 39));
    addCollideRect(rtp::CollideRect(62, 0, 62, 39));
    addCollideRect(rtp::CollideRect(0, 0, 61, 41));
}

void    Monster::BasicMonster::update(double) {

}

Monster::BasicMonster::~BasicMonster() = default;