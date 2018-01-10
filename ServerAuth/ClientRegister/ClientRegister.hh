//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_CLIENTREGISTER_HH
#define SERVERAUTH_CLIENTREGISTER_HH

# include "../BaseServer/BaseServer.hh"
# include "../Logger/Logger.hpp"
# include "RegisteredClient.hh"
# include "../ServerRegister/ServerRegister.hh"

namespace rtp {
    class ClientRegister : public BaseServer, private Logger<ClientRegister> {

    public:
        explicit ClientRegister(unsigned short, std::shared_ptr<IServerRegister>);
        ~ClientRegister() override;

    private:
        void    serverLooping() override;

    private:
        std::vector<std::shared_ptr<RegisteredClient> > _clientList;
        std::shared_ptr<IServerRegister> _iServerRegister;
    };
}

#endif //SERVERAUTH_CLIENTREGISTER_HH
