//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_ROOM_HH
#define SERVERAUTH_ROOM_HH

# include <memory>
# include <vector>
# include <thread>
# include "RegisteredClient.hh"
# include "../Logger/Logger.hpp"
#include "../ServerRegister/IServerRegister.hh"

namespace rtp {
    class Room : public Logger<Room> {
    public:
        explicit Room(unsigned int, std::shared_ptr<rtp::IServerRegister>&);
        ~Room();

    public:
        unsigned int    getId() const;
        bool    isOpen() const;
        bool    addPlayer(std::shared_ptr<RegisteredClient>&);
        bool    removePlayer(std::shared_ptr<RegisteredClient>&);
        bool    isPlayerIn(std::shared_ptr<RegisteredClient>&);
        unsigned long    nbPlayerIn() const;
        std::vector<std::shared_ptr<RegisteredClient> > const&  getPlayerList() const;
        bool    findAServer(std::shared_ptr<RegisteredClient>&);
        bool    stopMatchmaking(std::shared_ptr<RegisteredClient>&);


    private:
        unsigned int    _ownerId;
        unsigned int    _id;
        std::vector<std::shared_ptr<RegisteredClient> > _playerList;
        bool    _isOpen;
        bool    _haveAServer;

    private:
        std::unique_ptr<std::thread>    _matchmakingFinder;
        bool    _onMatchmaking;
        std::mutex  _locker;
        std::shared_ptr<IServerRegister>    _iServerRegister;

    private:
        static unsigned int _roomId;
    };
}

#endif //SERVERAUTH_ROOM_HH