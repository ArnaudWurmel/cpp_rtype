//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDCLIENT_HH
#define SERVERAUTH_REGISTEREDCLIENT_HH

# include <memory>
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/SocketReceiver.hh"
# include "../Logger/Logger.hpp"

namespace rtp {

    class   ClientRegister;

    class RegisteredClient : public NetworkAbstract::SocketReceiver, private Logger<RegisteredClient> {
    public:

    public:
        explicit RegisteredClient(std::shared_ptr<NetworkAbstract::ISocket>, ClientRegister&);
        ~RegisteredClient() override;

    public:
        bool    handleNewData();

    public:
        unsigned int    getId() const;

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;
        ClientRegister& _delegate;

    private:
        unsigned int    _id;

    private:
        static unsigned int _clientId;
    };
}

#endif //SERVERAUTH_REGISTEREDCLIENT_HH
