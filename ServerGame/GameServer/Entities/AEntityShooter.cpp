//
// Created by Arnaud WURMEL on 19/01/2018.
//

#include "AEntityShooter.hh"

rtp::AEntityShooter::AEntityShooter() {
    _needUpdate = false;
}

void    rtp::AEntityShooter::shoot(std::shared_ptr<rtp::ABullet> entity) {
    _entityList.push_back(entity);
}

void    rtp::AEntityShooter::updateSubEntities(double diff) {
    _needUpdate = false;
    auto iterator = _entityList.begin();

    while (iterator != _entityList.end()) {
        (*iterator)->update(diff);
        if ((*iterator)->isUpdated()) {
            _needUpdate = true;
        }
        ++iterator;
    }
}

bool    rtp::AEntityShooter::needUpdated() const {
    return _needUpdate;
}

std::vector<std::shared_ptr<rtp::ABullet> > rtp::AEntityShooter::getNeededUpdateEntities() {
    std::vector<std::shared_ptr<rtp::ABullet> > entitiesArray;
    auto iterator = _entityList.begin();

    while (iterator != _entityList.end()) {
        if ((*iterator)->isUpdated() || !(*iterator)->isSpawned() || (*iterator)->isExpectedToBeDeleted()) {
            (*iterator)->setUpdated(false);
            entitiesArray.push_back(*iterator);
        }
        if ((*iterator)->isExpectedToBeDeleted()) {
            _entityList.erase(iterator);
        }
        else {
            ++iterator;
        }
    }
    return entitiesArray;
}

rtp::AEntityShooter::~AEntityShooter() {}