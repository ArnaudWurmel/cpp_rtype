//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDSERVER_HH
#define SERVERAUTH_REGISTEREDSERVER_HH

# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/SocketReceiver.hh"

namespace rtp {
    class RegisteredServer : public NetworkAbstract::SocketReceiver {
    public:
        RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket>);
        ~RegisteredServer() override;

    public:
        bool    handleNewData();

    public:
        unsigned int    getId() const;

    private:
        unsigned int    _id;

    private:
        static unsigned int _serverId;
    };
}

#endif //SERVERAUTH_REGISTEREDSERVER_HH
