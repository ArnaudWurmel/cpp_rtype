//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "AEnemy.hh"

rtp::AEnemy::AEnemy(unsigned int id, std::string const& spriteName, int x, int y, unsigned int lifePoint) : AEntity(id, spriteName, x, y), ALivingEntity(lifePoint) {

}

bool rtp::AEnemy::isExpectedToBeDeleted() const {
    return !isAlive();
}

rtp::AEnemy::~AEnemy() {}