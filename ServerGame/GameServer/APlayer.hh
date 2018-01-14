//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef SERVERGAME_APLAYER_HH
#define SERVERGAME_APLAYER_HH

#include <string>
#include <functional>
#include <map>
#include "../NetworkAbstract/Message/Message.h"

namespace rtp {
    class APlayer {
    public:
        enum    Command {
            AUTHORIZE = 0
        };

    public:
        explicit APlayer(std::string const& authToken);
        virtual ~APlayer();

    public:
        bool    isAuthorized() const;
        bool    injectInput(NetworkAbstract::Message const&);

    private:
        bool    handleAuthorize(NetworkAbstract::Message const&);

    private:
        bool    _authorized;
        std::string _authToken;
        std::map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _functionPtrs;
    };
}

#endif //SERVERGAME_APLAYER_HH
