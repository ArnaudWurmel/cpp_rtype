#include <iostream>
#include <memory>
#include "BossMonster.hh"

#ifdef _WIN32
#define DllExport   __declspec( dllexport )

extern "C" DllExport rtp::AEnemy  *instanciateMonster(unsigned int, int, int);

DllExport rtp::AEnemy *instanciateMonster(unsigned int id, int x, int y) {
    return new Monster::BossMonster(id, x, y);
}

#else
extern "C" rtp::AEnemy  *instanciateMonster(unsigned int, int, int);

rtp::AEnemy *instanciateMonster(unsigned int id, int x, int y) {
    return new Monster::BossMonster(id, x, y);
}

#endif
