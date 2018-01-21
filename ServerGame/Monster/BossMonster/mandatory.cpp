#include <iostream>
#include <memory>
#include "BossMonster.hh"

extern "C" {
    rtp::AEnemy  *instanciateMonster(unsigned int, int, int);
}

rtp::AEnemy  *instanciateMonster(unsigned int id, int x, int y) {
    return new Monster::BossMonster(id, x, y);
}