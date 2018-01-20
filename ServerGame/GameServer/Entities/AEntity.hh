//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef SERVERGAME_AENTITY_HH
#define SERVERGAME_AENTITY_HH

# include <string>
#include <vector>
# include "CollideRect.hh"

# ifndef WIDTH
#  define WIDTH 600
# endif /* !WIDTH */

# ifndef HEIGHT
#  define HEIGHT 1000
# endif /* !HEIGHT */

namespace rtp {
    class   APlayer;
    class   AEntity {
    public:
        AEntity(std::string const&, int, int);
        virtual ~AEntity();

    public:
        virtual bool    collide(AEntity const&);
        virtual bool    isUpdated() const;
        void    setUpdated(bool);
        virtual std::string getInfos() const;
        virtual unsigned int    getEntityId() const;
        virtual bool    isExpectedToBeDeleted() const;
        virtual CollideRect  getCollideRect() const;

    public:
        virtual void    update(double) = 0;

    protected:
        virtual void    translate(Vector2<int> const&);
        void    addCollideRect(CollideRect const&);
        virtual std::string getInfoProtected() const;

    public:
        virtual std::string&    operator>>(std::string&) const;

    protected:
        std::vector<CollideRect> _collideRectList;

    protected:
        std::string _spriteName;
        int _currentFrame;
        Vector2<int>    _position;
        bool    _updated;
        unsigned int    _entityId;
        unsigned int    _rotation;

    private:
        static unsigned int _entityIdIncr;
    };
}

#endif //SERVERGAME_AENTITY_HH
