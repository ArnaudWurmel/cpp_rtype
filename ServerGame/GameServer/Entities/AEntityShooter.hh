//
// Created by Arnaud WURMEL on 19/01/2018.
//

#ifndef SERVERGAME_AENTITYSUMMONER_HH
#define SERVERGAME_AENTITYSUMMONER_HH

# include <memory>
# include <vector>
# include "AEntity.hh"
#include "Bullet/ABullet.hh"

namespace rtp {
    class AEntityShooter {
    public:
        AEntityShooter();
        virtual ~AEntityShooter();

    public:
        void    shoot(std::shared_ptr<rtp::ABullet>);
        void    updateSubEntities(double);
        std::vector<std::shared_ptr<rtp::ABullet> > getNeededUpdateEntities();

    private:
        std::vector<std::shared_ptr<rtp::ABullet> > _entityList;
    };
}


#endif //SERVERGAME_AENTITYSUMMONER_HH
