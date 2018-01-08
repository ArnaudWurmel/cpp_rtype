//
// Created by wurmel_a on 27/09/17.
//

#ifndef NETWORK_ISOCKET_H
#define NETWORK_ISOCKET_H

# include <queue>
# include <mutex>
# include <condition_variable>
# include "Message/Message.h"

namespace NetworkAbstract {
    class   ISocket {
    public:
        ISocket(std::mutex&, std::condition_variable&);
        virtual ~ISocket();

        virtual void    write(NetworkAbstract::Message message) = 0;
        virtual void    close() = 0;
        virtual bool    isOpen() const = 0;
        virtual bool    connectSocket(std::string const&, unsigned short port) = 0;
        virtual void    startSession() = 0;
        virtual std::string     getIpAddr() const = 0;
        virtual bool    haveAvailableData();
        NetworkAbstract::Message  getAvailableMessage();

    protected:
        void    addMessage(NetworkAbstract::Message);

    private:
        std::queue<NetworkAbstract::Message>  _messageList;
        std::mutex  _queueLocker;

    protected:
        std::mutex& _haveData;
        std::condition_variable&    _cv;
    };
}

#endif //NETWORK_ISOCKET_H
