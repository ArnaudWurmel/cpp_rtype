#include <iostream>
#include <memory>
#include "BasicMonster.hh"

extern "C" {
    rtp::AEnemy  *instanciateMonster(unsigned int, int, int);
}

rtp::AEnemy  *instanciateMonster(unsigned int id, int x, int y) {
    return new Monster::BasicMonster(id, x, y);
}