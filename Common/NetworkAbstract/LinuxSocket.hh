//
// Created by Arnaud WURMEL on 09/01/2018.
//

#ifndef SERVERAUTH_LINUXSOCKET_HH
#define SERVERAUTH_LINUXSOCKET_HH

# include <netinet/in.h>
# include "ISocket.h"


namespace NetworkAbstract {
    class LinuxSocket : public ISocket {
    public:
        LinuxSocket(std::condition_variable&, struct sockaddr_in& in, SOCKET socket = -1);
        LinuxSocket(std::condition_variable&, SOCKET socket = -1);
        ~LinuxSocket() override;

    public:
        bool    bind(unsigned short) override;
        void    flushWrite() override;
        void    close() override;
        void    read() override;
        bool    isOpen() const override;
        bool    connectSocket(std::string const&, unsigned short port) override;
        void    startSession() override;
        std::string     getIpAddr() const override;
        SOCKET& getSocket() override;
        SOCKET const&   getSocket() const override;

    private:
        Message _readM;

    private:
        SOCKET _socket;
        struct sockaddr_in _in;
        bool    _readHeader;
    };
}

#endif //SERVERAUTH_LINUXSOCKET_HH