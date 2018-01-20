//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "MonsterInstanciater.hh"

rtp::MonsterInstanciater::MonsterInstanciater() {
    _libFromMonsterType.insert(std::make_pair(MonsterType::BASIC, std::shared_ptr<ADLLManager>(ADLLManager::get("./Monster/BasicMonster/cmake-build-debug/libBasicMonster.dylib")));
}

std::vector<std::shared_ptr<rtp::AEnemy> >  rtp::MonsterInstanciater::instanciateWave(MonsterType type) {
    std::vector<std::shared_ptr<rtp::AEnemy> >  enemyList;
    int x = 0;
    int y = 0;

    if (_libFromMonsterType.find(type) != _libFromMonsterType.end()) {
        while (x < WIDTH) {
            AEnemy  *enemy = _libFromMonsterType[type]->getEnemyFromLib(x, y);

            if (enemy != nullptr) {
                std::cout << "Enemy instancied" << std::endl;
                delete enemy;
                return enemyList;
            }
            else {
                return enemyList;
            }
//            x = x + enemy->getCollideRect().getWidth();
 //           if (x < WIDTH) {
   //             enemyList.push_back(std::shared_ptr<AEnemy>(enemy));
     //       }
       //     else {
         //   }
        }
    }
    return enemyList;
}

rtp::MonsterInstanciater::~MonsterInstanciater() = default;