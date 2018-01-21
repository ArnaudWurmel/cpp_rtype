//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_ALIVINGENTITIES_HH
#define SERVERGAME_ALIVINGENTITIES_HH

namespace rtp {
    class ALivingEntity {
    public:
        explicit ALivingEntity(int lifePoint);
        ~ALivingEntity();

    public:
        bool    isAlive() const;
        void    hit(unsigned int damage);

    protected:
        int    _lifePoint;
    };
}


#endif //SERVERGAME_ALIVINGENTITIES_HH
