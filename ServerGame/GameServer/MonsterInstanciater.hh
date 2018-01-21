//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_MONSTERINSTANCIATER_HH
#define SERVERGAME_MONSTERINSTANCIATER_HH

#include <map>
# include "Entities/AEnemy.hh"
# include "DLLManager/ADLLManager.hh"

namespace rtp {
    class MonsterInstanciater {
    public:
        enum    MonsterType {
            BASIC = 0,
            BOSS = 1
        };

    public:
        MonsterInstanciater();
        ~MonsterInstanciater();

    public:
        std::vector<std::shared_ptr<AEnemy> >   instanciateWave(MonsterType);

    private:
        std::map<MonsterType, std::shared_ptr<rtp::ADLLManager> >  _libFromMonsterType;
    };
}


#endif //SERVERGAME_MONSTERINSTANCIATER_HH
