//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef SERVERGAME_AENTITY_HH
#define SERVERGAME_AENTITY_HH

# include <string>
#include <vector>
# include "CollideRect.hh"

namespace rtp {
    class   AEntity {
    public:
        AEntity(std::string const&, int, int);
        virtual ~AEntity();

    public:
        virtual bool    collide(AEntity const&);
        virtual bool    isUpdated() const;
        void    setUpdated(bool);
        virtual std::string getInfos() const;

    protected:
        virtual void    translate(Vector2<int> const&);
        virtual CollideRect  getCollideRect() const;
        void    addCollideRect(CollideRect const&);

    public:
        virtual std::string&    operator>>(std::string&) const = 0;

    protected:
        std::vector<CollideRect> _collideRectList;

    protected:
        std::string _spriteName;
        int _currentFrame;
        Vector2<int>    _position;
        bool    _updated;
    };
}

#endif //SERVERGAME_AENTITY_HH
