//
// Created by Arnaud WURMEL on 09/01/2018.
//

#ifndef SERVERGAME_GAMESERVER_HH
#define SERVERGAME_GAMESERVER_HH

# include <condition_variable>
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/IAcceptor.hh"

# define TOKEN_SIZE 32

namespace rtp {
    class GameServer {
    public:
        using Callback = bool   (GameServer::*)(NetworkAbstract::Message const&);

    public:
        enum Command {
            REGISTER = 0
        };

    public:
        GameServer(unsigned short);
        ~GameServer();

    public:
        bool    connectToAuthServer(std::string const&, unsigned short);
        bool    registerServer();

    private:
        bool    waitCommand(Callback, Command);

    private:
        bool    handleRegistering(NetworkAbstract::Message const&);

    private:
        std::condition_variable _inputAvailable;
        std::mutex  _inputLocker;

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;
        std::unique_ptr<NetworkAbstract::IAcceptor> _acceptor;
        unsigned short  _port;
        std::string _authToken;
    };
}

#endif //SERVERGAME_GAMESERVER_HH
