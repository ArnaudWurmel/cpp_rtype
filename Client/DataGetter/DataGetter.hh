//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_DATAGETTER_HH
#define CLIENTNETWORK_DATAGETTER_HH

# include <string>
# include <memory>
#include <thread>
# include "../NetworkAbstract/IAcceptor.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class DataGetter : private Logger<DataGetter> {
    public:
        using    Callback = bool (DataGetter::*)(NetworkAbstract::Message const&);

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
        bool    isRunning() const;

    private:
        bool    executeCommand(NetworkAbstract::Message const&, Callback);

        bool    handlePseudoSet(NetworkAbstract::Message const&);

    private:
        std::unique_ptr<NetworkAbstract::IAcceptor> _acceptor;
        std::shared_ptr<NetworkAbstract::ISocket>   _controlSocket;

    private:
        std::condition_variable _awaker;
        std::mutex _awakerLock;
    };
}

#endif //CLIENTNETWORK_DATAGETTER_HH
