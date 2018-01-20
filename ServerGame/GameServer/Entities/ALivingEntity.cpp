//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "ALivingEntity.hh"

rtp::ALivingEntity::ALivingEntity(int lifePoint) {
    _lifePoint = lifePoint;
}

void rtp::ALivingEntity::hit(unsigned int damage) {
    _lifePoint -= damage;
}

bool    rtp::ALivingEntity::isAlive() const {
    return _lifePoint > 0;
}

rtp::ALivingEntity::~ALivingEntity() {}