//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_SOCKETRECEIVER_HH
#define SERVERAUTH_SOCKETRECEIVER_HH

# include "ISocket.h"

namespace NetworkAbstract {
    class SocketReceiver {
    public:
        SocketReceiver(std::shared_ptr<ISocket>);
        virtual ~SocketReceiver();

    public:
        virtual void    write(NetworkAbstract::Message message);
        virtual void    close();
        virtual bool    isOpen() const;
        virtual std::string     getIpAddr() const;
        std::shared_ptr<ISocket> const& getSocket() const;


    protected:
        std::shared_ptr<ISocket>    _socket;
    };
}

#endif //SERVERAUTH_SOCKETRECEIVER_HH
