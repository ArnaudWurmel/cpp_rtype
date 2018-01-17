//
// Created by Arnaud WURMEL on 09/01/2018.
//

#ifndef SERVERGAME_GAMESERVER_HH
#define SERVERGAME_GAMESERVER_HH

# include <thread>
# include <chrono>
# include <condition_variable>
# include <string>
#include <map>
# include "../Logger/Logger.hpp"
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/IAcceptor.hh"
#include "../NetworkAbstract/IServer.hh"
#include "Entity/APlayer.hh"

# define TOKEN_SIZE 32

namespace rtp {
    class GameServer : private Logger<GameServer> {
    public:
        using Callback = bool   (GameServer::*)(NetworkAbstract::Message const&);

    public:
        enum Command {
            REGISTER = 0,
            PING = 1,
            RESERVED = 2
        };

        enum    ServerState {
            Available = 0,
            Busy = 1,
            Unknown = 2,
            NotRegistered = 3
        };


    public:
        GameServer(unsigned short);
        ~GameServer();

    public:
        bool    connectToAuthServer(std::string const&, unsigned short);
        bool    registerServer();
        void    serverLoop();

    private:
        bool    handleMessage(NetworkAbstract::Message const&);
        bool    waitCommand(Callback, Command);
        void    runGame();

        //
        // Callback after I send a command
        //
    private:
        bool    handleRegistering(NetworkAbstract::Message const&);

        //
        // Callback after receive a command
        //
    private:
        bool    handlePing(NetworkAbstract::Message const&);
        bool    handleReserved(NetworkAbstract::Message const&);

    private:
        std::condition_variable _inputAvailable;
        std::mutex  _inputLocker;

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;
        std::shared_ptr<NetworkAbstract::IServer<NetworkAbstract::BoostUdpClient<APlayer> > >   _gameServer;
        unsigned short  _port;

    private:
        std::string _authToken;
        std::map<Command, Callback> _callbackPtrs;
        std::map<ServerState, std::string> _stateTranslator;
        ServerState _serverState;
        std::chrono::time_point<std::chrono::steady_clock> _lockedAt;
        std::unique_ptr<std::thread>    _gameRunner;
        bool    _gameRunning;
        std::mutex  _pLock;
    };
}

#endif //SERVERGAME_GAMESERVER_HH
