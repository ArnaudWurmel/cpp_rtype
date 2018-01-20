//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef BASICMONSTER_BASICMONSTER_HH
#define BASICMONSTER_BASICMONSTER_HH

# include "Entities/AEntity.hh"
# include "Entities/ALivingEntity.hh"
# include "Entities/AEnemy.hh"

namespace Monster {
    class BasicMonster : public rtp::AEnemy {
    public:
        BasicMonster(int x, int y);
        ~BasicMonster() override;

    public:
        void    update(double) override;
    };
}

#endif //BASICMONSTER_BASICMONSTER_HH
