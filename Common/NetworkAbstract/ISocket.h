//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKET_H
#define NETWORK_ISOCKET_H

# include <queue>
# include <mutex>
# include <condition_variable>
# include "Message/Message.h"

#if !_WIN32
    typedef int SOCKET;
#endif

namespace NetworkAbstract {
    class   ISocket {
    public:
        ISocket(std::condition_variable&);
        virtual ~ISocket();

        virtual bool    bind(unsigned short) = 0;
        virtual void    write(NetworkAbstract::Message message);
        virtual void    read() = 0;
        virtual void    close() = 0;
        virtual bool    isOpen() const = 0;
        virtual bool    connectSocket(std::string const&, unsigned short port) = 0;
        virtual void    startSession() = 0;
        virtual std::string     getIpAddr() const = 0;
        virtual SOCKET& getSocket() = 0;
        virtual SOCKET const&   getSocket() const = 0;
        virtual bool    haveAvailableData();
        virtual bool    haveSomethingToWrite();
        virtual void    flushWrite() = 0;
        NetworkAbstract::Message  getAvailableMessage();

    protected:
        void    addMessage(NetworkAbstract::Message);

    protected:
        std::queue<NetworkAbstract::Message>    _writingList;
        std::queue<NetworkAbstract::Message>  _messageList;
        std::mutex  _queueLocker;
        std::mutex  _writingLocker;

    protected:
        std::condition_variable&    _cv;
    };
}

#endif //NETWORK_ISOCKET_H
