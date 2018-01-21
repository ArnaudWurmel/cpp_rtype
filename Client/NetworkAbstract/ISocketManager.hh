//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_ISOCKETMANAGER_HH
#define SERVERGAME_ISOCKETMANAGER_HH

# include "ISocket.h"

namespace NetworkAbstract {
    class   ISocketManager {
    public:
        enum SocketType {
            UDP = 0,
            TCP = 1
        };

    public:
        virtual ~ISocketManager() = default;

        virtual std::shared_ptr<NetworkAbstract::ISocket>   getSocket(SocketType, std::condition_variable&) = 0;
        virtual void    enableSocket() = 0;
        virtual void    disableSocket() = 0;
        virtual bool    isEnabled() const = 0;
    };
}

#endif //SERVERGAME_ISOCKETMANAGER_HH
