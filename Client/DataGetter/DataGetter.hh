//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_DATAGETTER_HH
#define CLIENTNETWORK_DATAGETTER_HH

# include <string>
# include <memory>
# include <thread>
# include <functional>
# include "../NetworkAbstract/IAcceptor.hh"
# include "../Logger/Logger.hpp"
# include "Room.hh"

namespace rtp {
    class DataGetter : private Logger<DataGetter> {
    public:
        using    Callback = bool (DataGetter::*)(NetworkAbstract::Message const&);
        using    Emptier = std::function<void   (NetworkAbstract::Message const&)>;
        using    EmptierFrom = std::function<void (std::shared_ptr<NetworkAbstract::ISocket>, NetworkAbstract::Message const&)>;

    public:
        enum Command {
            CreateRoom = 0,
            LeaveRoom = 1,
            RoomList = 2,
            JoinRoom = 3,
            StartMatchmaking = 4,
            StopMatchmaking = 5,
            ServerFound = 6,
            NewOwner = 7,
            SetPseudo = 8
        };

    public:
        DataGetter();
        ~DataGetter() override;

    public:
        bool    connectToHost(std::string const& host);
        bool    setPseudo(std::string const&);
        bool    updateRoomList();
        int     createRoom();
        bool    leaveRoom();
        bool    isRunning() const;
        bool    startMatchmaking();
        bool    stopMatchmaking();
        void    reset();
        void    emptyMessage(Emptier);
        bool    joinRoom(int roomId);

    public:
        std::string const&  getPseudo() const;
        std::vector<std::unique_ptr<Room> > const&  getRoomList() const;
        std::shared_ptr<NetworkAbstract::ISocket>   getUdpSocket(std::condition_variable&);

    private:
        bool    executeCommand(NetworkAbstract::Message const&, Callback);

        bool    handlePseudoSet(NetworkAbstract::Message const&);
        bool    handleRoomList(NetworkAbstract::Message const&);
        bool    handleCreateRoom(NetworkAbstract::Message const&);
        bool    handleLeaveRoom(NetworkAbstract::Message const&);
        bool    handleStartMatchmaking(NetworkAbstract::Message const&);
        bool    handleStopMatchmaking(NetworkAbstract::Message const&);
        bool    handleJoinRoom(NetworkAbstract::Message const&);

    public:
        static std::vector<std::string> getTokenFrom(std::string const&, char sep = ' ');
        static bool authorizeClient(std::shared_ptr<NetworkAbstract::ISocket>, EmptierFrom, std::string const&, std::string const&);
        static bool waitCommandExecution(std::shared_ptr<NetworkAbstract::ISocket>, NetworkAbstract::Message const&, EmptierFrom);

    private:
        std::unique_ptr<NetworkAbstract::IAcceptor> _acceptor;
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;

    private:
        std::condition_variable _awaker;
        std::mutex _awakerLock;

    private:
        std::string _pseudo;
        std::vector<std::unique_ptr<Room> > _roomList;
        int _roomId;
    };
}

#endif //CLIENTNETWORK_DATAGETTER_HH
