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
    class APlayer : AEntity {
    public:
        using OnAuthorization = std::function<void  (bool)>;

    public:
        enum    Command {
            AUTHORIZE = 0,
            SPAWN_PLAYER = 1,
            UPDATE_BOARD = 2
        };

    public:
        explicit APlayer(OnAuthorization const& authorizationCallback, std::string const& authToken);
        virtual ~APlayer() override;

    public:
        bool    isAuthorized() const;
        bool    injectInput(NetworkAbstract::Message const&);
        std::string const&  getPseudo() const;
        unsigned int    getId() const;

    public:
        std::string& operator>>(std::string& dest) const override;

    private:
        bool    handleAuthorize(NetworkAbstract::Message const&);

    private:
        static std::vector<std::string> getTokenFrom(std::string const&, char);

    private:
        OnAuthorization _authorizationCallback;
        bool    _authorized;
        std::string _pseudo;
        std::string _authToken;
        std::map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _functionPtrs;
        unsigned int    _id;

    private:
        static unsigned int _clientIdIncr;
    };
}

#endif //SERVERGAME_APLAYER_HH
