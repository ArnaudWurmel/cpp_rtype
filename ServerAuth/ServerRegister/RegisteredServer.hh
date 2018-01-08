//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDSERVER_HH
#define SERVERAUTH_REGISTEREDSERVER_HH

# include "../NetworkAbstract/ISocket.h"

namespace rtp {
    class RegisteredServer {
    public:
        RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket>);
        ~RegisteredServer();

    public:
        bool    handleNewData();

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _socketControl;
    };
}

#endif //SERVERAUTH_REGISTEREDSERVER_HH
