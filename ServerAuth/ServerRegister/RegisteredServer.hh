//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDSERVER_HH
#define SERVERAUTH_REGISTEREDSERVER_HH

# include <functional>
# include <set>
# include <list>
# include <unordered_map>
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/SocketReceiver.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class RegisteredServer : public NetworkAbstract::SocketReceiver, private Logger<RegisteredServer> {
    public:
        enum Command {
            REGISTER = 0
        };

    public:
        RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket>);
        ~RegisteredServer() override;

    public:
        bool    handleNewData();

    public:
        unsigned int    getId() const;
        bool    isRegistered() const;

    private:
        bool    registerServer(std::vector<std::string> const&);

    private:
        unsigned int    _id;
        int _port;

    private:
        static unsigned int _serverId;
        std::unordered_map<Command, std::function<bool (std::vector<std::string> const&)> >   _funcPtrList;
    };
}

#endif //SERVERAUTH_REGISTEREDSERVER_HH
