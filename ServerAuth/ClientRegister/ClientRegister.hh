//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_CLIENTREGISTER_HH
#define SERVERAUTH_CLIENTREGISTER_HH

# include "../BaseServer/BaseServer.hh"
# include "RegisteredClient.hh"
# include "../ServerRegister/ServerRegister.hh"
# include "Room.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class ClientRegister : public BaseServer, private Logger<ClientRegister> {

    public:
        explicit ClientRegister(unsigned short, std::shared_ptr<IServerRegister>);
        ~ClientRegister() override;

    public:
        int getPlayerRoomId(std::shared_ptr<RegisteredClient>) const;
        bool    playerCreateRoom(std::shared_ptr<RegisteredClient>);
        bool    playerLeaveRoom(std::shared_ptr<RegisteredClient>);
        bool    playerJoinRoom(std::shared_ptr<RegisteredClient>, int);
        bool    playerStartMatchmaking(std::shared_ptr<RegisteredClient>);
        bool    playerStopMatchmaking(std::shared_ptr<RegisteredClient>);
        std::vector<std::unique_ptr<Room> > const&  playerAskRoomList() const;

    private:
        void    serverLooping() override;

    private:
        std::vector<std::shared_ptr<RegisteredClient> > _clientList;
        std::shared_ptr<IServerRegister> _iServerRegister;
        std::vector<std::unique_ptr<Room> > _roomList;
    };
}

#endif //SERVERAUTH_CLIENTREGISTER_HH
