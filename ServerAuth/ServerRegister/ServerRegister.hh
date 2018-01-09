//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_SERVERREGISTER_HH
#define SERVERAUTH_SERVERREGISTER_HH

# include <string>
# include <thread>
# include <memory>
# include "../Logger/Logger.hpp"
# include "../ArgumentLoader/ArgumentLoader.hh"
# include "RegisteredServer.hh"
# include "../NetworkAbstract/LinuxSocket.hh"
#include "../NetworkAbstract/IAcceptor.hh"

namespace rtp {
    class ServerRegister : private Logger<ServerRegister> {

    public:
        explicit ServerRegister(unsigned short port = 8080);
        ~ServerRegister() override;

    public:
        bool    isRunning() const;
        void    stop();

    public:
        std::vector<std::shared_ptr<RegisteredServer> > const& getServer() const;

    private:
        void    serverLooping();

        ///////////////
        // Server's Management
        //
    private:
        std::vector<std::shared_ptr<RegisteredServer> > _serverList;

        ///////////////
        //  NetworkManagement
        //
    private:
        std::unique_ptr<NetworkAbstract::IAcceptor> _acceptor;
        std::unique_ptr<std::thread>    _thread;

    private:
        bool    _threadRunning;
        std::mutex  _mClient;
        std::condition_variable _clientNotifier;
    };
}

#endif //SERVERAUTH_SERVERREGISTER_HH
