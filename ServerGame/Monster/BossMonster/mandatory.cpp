#include <iostream>
#include <memory>
#include "BossMonster.hh"

#define DllExport   __declspec( dllexport )   

extern "C" DllExport rtp::AEnemy  *instanciateMonster(unsigned int, int, int);

DllExport rtp::AEnemy *instanciateMonster(unsigned int id, int x, int y) {
    return new Monster::BossMonster(id, x, y);
}