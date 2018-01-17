//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef CLIENTNETWORK_GAMEHANDLER_HH
#define CLIENTNETWORK_GAMEHANDLER_HH

# include "Entities/Player.hh"
# include "../NetworkAbstract/ISocket.h"

namespace rtp {
    class GameHandler {
    public:
        enum    Command {
            AUTHORIZE = 0,
            SPAWN_PLAYER = 1,
            FORWARD = 2,
            BACKWARD = 3,
            LEFT = 4,
            RIGHT = 5,
            UPDATE_PLAYER = 6
        };

    public:
        GameHandler(std::shared_ptr<NetworkAbstract::ISocket>, std::shared_ptr<Player>);
        ~GameHandler();

    public:
        bool    update(sf::RenderWindow& window);

    private:
        bool    handlePlayerSpawn(std::string const&);
        bool    handleUpdatePlayer(std::string const&);

    private:
        void    handleTouchIsPressed();
        void    forward();
        void    backward();
        void    left();
        void    right();

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _gameSocket;
        std::vector<std::shared_ptr<Player> >   _playerList;
        std::chrono::time_point<std::chrono::system_clock>       _lastMessage;

    private:
        std::map<Command, std::function<bool (std::string const&)> >    _callbackList;
    };
}

#endif //CLIENTNETWORK_GAMEHANDLER_HH
