//
// Created by Arnaud WURMEL on 09/01/2018.
//

#ifndef SERVERGAME_GAMESERVER_HH
#define SERVERGAME_GAMESERVER_HH

# include <chrono>
# include <condition_variable>
# include <string>
# include <map>
#include <thread>
# include "../Logger/Logger.hpp"
# include "../NetworkAbstract/ISocketManager.hh"
#include "../NetworkAbstract/IUdpInputManager.hh"
#include "Entities/APlayer.hh"
# include "MonsterInstanciater.hh"

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
        explicit GameServer(unsigned short);
        ~GameServer() override;

    public:
        bool    connectToAuthServer(std::string const&, unsigned short);
        bool    registerServer();
        void    serverLoop();

    private:
        bool    handleMessage(NetworkAbstract::Message const&);
        bool    waitCommand(Callback, Command);

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
        void    handleGame();

    public:
        static std::vector<std::string> getTokenFrom(std::string const&, char sep = ' ');

    private:
        std::condition_variable _inputAvailable;
        std::mutex  _inputLocker;

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;
        std::unique_ptr<NetworkAbstract::ISocketManager>    _socketManager;
        std::unique_ptr<NetworkAbstract::IUdpInputManager>  _inputManager;
        unsigned short  _port;

    private:
        bool    _gameRunning;
        std::unique_ptr<std::thread>    _gameManager;
        std::string _authToken;
        std::map<Command, Callback> _callbackPtrs;
        std::map<ServerState, std::string> _stateTranslator;
        ServerState _serverState;
        std::chrono::time_point<std::chrono::system_clock> _lockedAt;
        MonsterInstanciater _monsterInstanciater;
    };
}

#endif //SERVERGAME_GAMESERVER_HH
