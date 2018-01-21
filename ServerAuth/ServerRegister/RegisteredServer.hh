//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_REGISTEREDSERVER_HH
#define SERVERAUTH_REGISTEREDSERVER_HH

# include <functional>
# include <map>
# include <string>
# include <set>
# include <list>
# include <unordered_map>
# include <chrono>
#ifdef _WIN32
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif // _WIN32
# include "../NetworkAbstract/ISocket.h"
# include "../NetworkAbstract/SocketReceiver.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class RegisteredServer : public NetworkAbstract::SocketReceiver, private Logger<RegisteredServer> {
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
        RegisteredServer(std::shared_ptr<NetworkAbstract::ISocket>);
        ~RegisteredServer() override;

    public:
        bool    handleNewData();

    public:
        unsigned int    getId() const;
        int  getPort() const;
        bool    isRegistered() const;
        ServerState getState() const;
        std::string getStateTranslated();
        std::string const&  getRegistrationToken() const;

    private:
        bool    registerServer(NetworkAbstract::Message const&);
        bool    pingResult(NetworkAbstract::Message const&);

    private:
        std::string generateRandomString(ssize_t) const;
        ServerState getStateFromString(std::string const&) const;

    private:
        std::string     _registrationToken;
        ServerState     _serverState;
        unsigned int    _id;
        int _port;
        std::chrono::time_point<std::chrono::system_clock>  _lastPing;

    private:
        static unsigned int _serverId;
        std::unordered_map<Command, std::function<bool (NetworkAbstract::Message const&)> >   _funcPtrList;
        std::map<ServerState, std::string>  _stateServerTranslate;
    };
}

#endif //SERVERAUTH_REGISTEREDSERVER_HH
