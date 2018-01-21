//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef BASICMONSTER_BASICMONSTER_HH
#define BASICMONSTER_BASICMONSTER_HH

# include "Entities/AEntity.hh"
# include "Entities/ALivingEntity.hh"
# include "Entities/AEnemy.hh"

namespace Monster {
    class BossMonster : public rtp::AEnemy {
    public:
        BossMonster(unsigned int id, int x, int y);
        ~BossMonster() override;

    public:
        void    update(double) override;

    private:
        unsigned int    _frame;
    };
}

#endif //BASICMONSTER_BASICMONSTER_HH
