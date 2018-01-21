//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "ALivingEntity.hh"

rtp::ALivingEntity::ALivingEntity(int lifePoint) {
    _lifePoint = lifePoint;
}

void rtp::ALivingEntity::hit(unsigned int damage) {
    _lifePoint -= damage;
    std::cout << _lifePoint << std::endl;
}

bool    rtp::ALivingEntity::isAlive() const {
    return _lifePoint > 0;
}

rtp::ALivingEntity::~ALivingEntity() {}