//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_ROOM_HH
#define SERVERAUTH_ROOM_HH

# include <memory>
# include <vector>
# include "RegisteredClient.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class Room : public Logger<Room> {
    public:
        explicit Room(unsigned int);
        ~Room();

    public:
        unsigned int    getId() const;
        bool    addPlayer(std::shared_ptr<RegisteredClient>&);
        bool    removePlayer(std::shared_ptr<RegisteredClient>&);
        bool    isPlayerIn(std::shared_ptr<RegisteredClient>&);
        unsigned long    nbPlayerIn() const;

    private:
        unsigned int    _ownerId;
        unsigned int    _id;
        std::vector<std::shared_ptr<RegisteredClient> > _playerList;

    private:
        static unsigned int _roomId;
    };
}

#endif //SERVERAUTH_ROOM_HH