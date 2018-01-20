//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_ALLYBULLET_HH
#define SERVERGAME_ALLYBULLET_HH

# include "ABullet.hh"

namespace rtp {
    class AllyBullet : public ABullet {
    public:
        AllyBullet(Vector2<int>, unsigned int damage);
        ~AllyBullet() override;

    public:
        void    onCollide(APlayer&) override;

    private:
        unsigned int _damage;
    };
}

#endif //SERVERGAME_ALLYBULLET_HH
