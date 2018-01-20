#include <iostream>
#include <memory>
#include "BasicMonster.hh"

extern "C" {
    rtp::AEnemy  *instanciateMonster(int, int);
}

rtp::AEnemy  *instanciateMonster(int x, int y) {
    return new Monster::BasicMonster(x, y);
}