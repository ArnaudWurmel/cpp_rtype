//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_AENEMY_HH
#define SERVERGAME_AENEMY_HH

# include "AEntity.hh"
# include "ALivingEntity.hh"

namespace rtp {
    class   AEnemy : public AEntity, public ALivingEntity {
    public:
        AEnemy(std::string const&, int, int, unsigned int);
        ~AEnemy() override;

    public:
        void    update(double) override = 0;
    };
}

#endif //SERVERGAME_AENEMY_HH
