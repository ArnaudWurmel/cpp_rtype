//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_SERVERREGISTER_HH
#define SERVERAUTH_SERVERREGISTER_HH

# include <string>
# include <thread>
# include <memory>
# include "RegisteredServer.hh"
# include "../BaseServer/BaseServer.hh"
# include "IServerRegister.hh"
#ifdef _WIN32
# include "../LoggerWin/Logger.hpp"
# include "../ArgumentLoaderWin/ArgumentLoader.hh"
# include "../NetworkAbstractWin/IAcceptor.hh"
#else
# include "../Logger/Logger.hpp"
# include "../ArgumentLoader/ArgumentLoader.hh"
# include "../NetworkAbstract/IAcceptor.hh"
#endif // _WIN32

namespace rtp {
    class ServerRegister : public BaseServer, private Logger<ServerRegister>, public IServerRegister {

    public:
        explicit ServerRegister(unsigned short port = 8080);
        ~ServerRegister() override;

    public:
        std::vector<std::shared_ptr<RegisteredServer> > const& getServer() const override;
        void    lockData() override;
        void    unlockData() override;

    private:
        void    serverLooping() override;

        ///////////////
        // Server's Management
        //
    private:
        std::mutex  _dataSafer;
        std::vector<std::shared_ptr<RegisteredServer> > _serverList;
    };
}

#endif //SERVERAUTH_SERVERREGISTER_HH
