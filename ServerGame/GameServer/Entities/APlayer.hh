//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef SERVERGAME_APLAYER_HH
#define SERVERGAME_APLAYER_HH

#include <string>
#include <functional>
#include <map>
#ifdef _WIN32
#include "../../NetworkAbstractWin/Message/Message.h"
#else
#include "../../NetworkAbstract/Message/Message.h"
#endif // _WIN32
#include "AEntity.hh"
#include "AEntityShooter.hh"
#include "ALivingEntity.hh"

namespace rtp {
    class APlayer : public AEntity, public AEntityShooter, public ALivingEntity {

    public:
        enum    Movement {
            mapForward = 0,
            mapBackward = 1,
            mapLeft = 2,
            mapRight = 3
        };

    public:
        enum    AnimationName {
            FullLeft = 0,
            Left = 1,
            Center = 2,
            Right = 3,
            FullRight = 4
        };
    public:
        enum    Command {
            AUTHORIZE = 0,
            SPAWN_PLAYER = 1,
            FORWARD = 2,
            BACKWARD = 3,
            LEFT = 4,
            RIGHT = 5,
            UPDATE_PLAYER = 6,
            SPAWN_ENTITY = 7,
            UPDATE_ENTITY = 8,
            DELETE_ENTITY = 9
        };

    public:
        APlayer();
        virtual ~APlayer() override;

    public:
        bool    isAuthorized() const;
        void    setAuthorize(bool);
        virtual bool    injectInput(NetworkAbstract::Message const&);
        std::string const&  getPseudo() const;
        unsigned int    getId() const;
        void    update(double) override;
        void    setPseudo(std::string const&);
        std::string getInfos() const override;

    public:
        std::string& operator>>(std::string& dest) const override;

    private:
        bool    handleForward(NetworkAbstract::Message const&);
        bool    handleBackward(NetworkAbstract::Message const&);
        bool    handleLeft(NetworkAbstract::Message const&);
        bool    handleRight(NetworkAbstract::Message const&);

    private:
        void    forward(double diff);
        void    backward(double diff);
        void    left(double diff);
        void    right(double diff);
        void    collideForward();
        void    collideBackward();
        void    collideLeft();
        void    collideRight();

    private:
        bool    _authorized;
        std::string _pseudo;
        std::map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _functionPtrs;
        std::vector<std::pair<bool, std::function<void (double)> > > _moveMapping;
        unsigned int    _id;
        unsigned int    _noUpdatedCount;
        unsigned int    _lastShoot;

    private:
        unsigned int    _modifierFrameIncr;

    private:
        static unsigned int _clientIdIncr;
    };
}

#endif //SERVERGAME_APLAYER_HH
