//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_IACCEPTOR_HH
#define SERVERAUTH_IACCEPTOR_HH

# include "ISocket.h"

namespace NetworkAbstract {
    class   IAcceptor {
    public:
        virtual ~IAcceptor() = default;

    public:
        virtual bool startAccept() = 0;
        virtual bool haveAwaitingClient() = 0;
        virtual std::shared_ptr<NetworkAbstract::ISocket>   acceptClient() = 0;
        virtual void    run() = 0;
        virtual void    stop() = 0;

    public:
        static void    newFd(int fd);
        static unsigned int    getMaxFd();
        static void    deleteFd(int fd);

    private:
        static std::vector<unsigned int>    _maxFd;
        static std::mutex  _lock;
    };
}

#endif //SERVERAUTH_IACCEPTOR_HH
