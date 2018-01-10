//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDCLIENT_HH
#define SERVERAUTH_REGISTEREDCLIENT_HH

# include <memory>
# include <unordered_map>
# include <functional>
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/SocketReceiver.hh"
# include "../Logger/Logger.hpp"

namespace rtp {

    class   ClientRegister;

    class RegisteredClient : public NetworkAbstract::SocketReceiver, private Logger<RegisteredClient>, public std::enable_shared_from_this<RegisteredClient> {
    public:
        enum Command {
            CreateRoom = 0,
            LeaveRoom = 1,
            RoomList = 2,
            JoinRoom = 3
        };

    public:
        explicit RegisteredClient(std::shared_ptr<NetworkAbstract::ISocket>, ClientRegister&);
        ~RegisteredClient() override;

    public:
        bool    handleNewData();

    private:
        bool    createRoom(NetworkAbstract::Message const&);
        bool    leaveRoom(NetworkAbstract::Message const&);
        bool    roomList(NetworkAbstract::Message const&);
        bool    joinRoom(NetworkAbstract::Message const&);

    public:
        unsigned int    getId() const;

    private:
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;
        ClientRegister& _delegate;
        std::unordered_map<Command, std::function<bool  (NetworkAbstract::Message const&)> >    _commandCallback;

    private:
        unsigned int    _id;

    private:
        static unsigned int _clientId;
    };
}

#endif //SERVERAUTH_REGISTEREDCLIENT_HH
