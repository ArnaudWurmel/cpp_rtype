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
        APlayer();
        virtual ~APlayer();

    public:
        bool    isAuthorized() const;
        virtual bool    injectInput(NetworkAbstract::Message const&);

    protected:
        void    setAuthorize(bool authorize);

    private:
        bool    _authorized;
        std::map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _functionPtrs;
    };
}

#endif //SERVERGAME_APLAYER_HH
