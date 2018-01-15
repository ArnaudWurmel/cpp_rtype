//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef SERVERGAME_AENTITY_HH
#define SERVERGAME_AENTITY_HH

# include <string>
# include "CollideRect.hh"

namespace rtp {
    class   AEntity {
    public:
        AEntity(std::string const&, int nbFrame, CollideRect const&);
        virtual ~AEntity();

    protected:
        virtual void    translate(Vector2<int> const&);
        virtual bool    collide(AEntity const&);
        virtual CollideRect const&  getCollideRect() const;

    public:
        virtual std::string&    operator>>(std::string&) const = 0;

    private:
        CollideRect _collideRect;

    protected:
        std::string _spriteName;
        int _nbFrame;
        int _currentFrame;
    };
}

#endif //SERVERGAME_AENTITY_HH
