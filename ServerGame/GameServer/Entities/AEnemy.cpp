//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "AEnemy.hh"

rtp::AEnemy::AEnemy(std::string const& spriteName, int x, int y, unsigned int lifePoint) : AEntity(spriteName, x, y), ALivingEntity(lifePoint) {

}

rtp::AEnemy::~AEnemy() {}