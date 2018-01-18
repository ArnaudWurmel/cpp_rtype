//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef SERVERGAME_APLAYER_HH
#define SERVERGAME_APLAYER_HH

#include <string>
#include <functional>
#include <map>
#include "../../NetworkAbstract/Message/Message.h"
#include "AEntity.hh"

namespace rtp {
    class APlayer : public AEntity {

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
            UPDATE_PLAYER = 6
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
        void    handleMoving(float);
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
        void    forward(float diff);
        void    backward(float diff);
        void    left(float diff);
        void    right(float diff);
        void    resetAnimation();

    private:
        bool    _authorized;
        std::string _pseudo;
        std::map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _functionPtrs;
        std::vector<std::pair<bool, std::function<void (float)> > > _moveMapping;
        unsigned int    _id;
        unsigned int    _noUpdatedCount;

    private:
        unsigned int    _modifierFrameIncr;

    private:
        static unsigned int _clientIdIncr;
    };
}

#endif //SERVERGAME_APLAYER_HH
