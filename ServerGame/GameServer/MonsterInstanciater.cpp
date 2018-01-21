//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "MonsterInstanciater.hh"
#include "../Exceptions/Exception.hh"

rtp::MonsterInstanciater::MonsterInstanciater() {
    _libFromMonsterType.insert(std::make_pair(MonsterType::BASIC, std::shared_ptr<ADLLManager>(ADLLManager::get("./Monster/BasicMonster/cmake-build-debug/libBasicMonster"))));
    auto    iterator = _libFromMonsterType.begin();

    while (iterator != _libFromMonsterType.end()) {
        if (!(*iterator).second->init()) {
            throw rtp::MissingFiles();
        }
        ++iterator;
    }
}

std::vector<std::shared_ptr<rtp::AEnemy> >  rtp::MonsterInstanciater::instanciateWave(MonsterType type) {
    std::vector<std::shared_ptr<rtp::AEnemy> >  enemyList;
    int x = 0;
    int y = 0;

    if (_libFromMonsterType.find(type) != _libFromMonsterType.end()) {
        while (x < WIDTH) {
            AEnemy  *enemy = _libFromMonsterType[type]->getEnemyFromLib(AEntity::getNextId(), x, y);

            if (enemy != nullptr) {
                x = x + enemy->getCollideRect().getWidth() + 2;
                if (x < WIDTH) {
                    enemyList.push_back(std::shared_ptr<AEnemy>(enemy));
                }
                else {
                    delete enemy;
                }
            }
            else {
                return enemyList;
            }
        }
    }
    return enemyList;
}

rtp::MonsterInstanciater::~MonsterInstanciater() = default;