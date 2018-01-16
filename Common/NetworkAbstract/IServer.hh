//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef SERVERGAME_ISERVER_HH
#define SERVERGAME_ISERVER_HH

#include "ISocket.h"
#include "BoostUdpClient.hpp"

namespace NetworkAbstract {
    template<typename T> class   IServer {
    public:
        virtual ~IServer() = default;

        virtual void init(std::string const&) = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
        virtual void close() = 0;
        virtual void acceptIncommingConnexion(bool) = 0;
        virtual std::shared_ptr<ISocket>    getEmptyASocket(std::condition_variable&) = 0;
        virtual std::vector<std::shared_ptr<T> > const& getClient() const = 0;
        virtual std::vector<std::shared_ptr<T> >&   getClient() = 0;
    };
}

#endif //SERVERGAME_ISERVER_HH
