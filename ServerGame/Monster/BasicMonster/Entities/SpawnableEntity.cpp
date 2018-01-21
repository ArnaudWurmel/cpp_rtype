//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "SpawnableEntity.hh"

rtp::SpawnableEntity::SpawnableEntity() {
    _isSpawned = false;
}

bool    rtp::SpawnableEntity::isSpawned() const {
    return _isSpawned;
}

void    rtp::SpawnableEntity::spawn() {
    _isSpawned = true;
}

rtp::SpawnableEntity::~SpawnableEntity() = default;