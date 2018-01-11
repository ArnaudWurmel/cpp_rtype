//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_ISERVERREGISTER_HH
#define SERVERAUTH_ISERVERREGISTER_HH

# include "RegisteredServer.hh"

namespace   rtp {
    class   IServerRegister {
    public:
        virtual ~IServerRegister() = default;

    public:
        virtual std::vector<std::shared_ptr<RegisteredServer> > const& getServer() const = 0;
        virtual void    lockData() = 0;
        virtual void    unlockData() = 0;
    };
}

#endif //SERVERAUTH_ISERVERREGISTER_HH
