//
// Created by Arnaud WURMEL on 10/01/2018.
//

#ifndef SERVERAUTH_BASESERVER_HH
#define SERVERAUTH_BASESERVER_HH

# include <thread>
# include <mutex>
# include "../NetworkAbstract/IAcceptor.hh"

namespace rtp {
    class BaseServer {
    public:
        explicit    BaseServer(unsigned short port);
        virtual ~BaseServer();

    public:
        bool    isRunning() const;
        void    stop();

    public:
        virtual void    serverLooping();

        ///////////////
        //  NetworkManagement
        //
    protected:
        std::unique_ptr<NetworkAbstract::IAcceptor> _acceptor;
        std::unique_ptr<std::thread>    _thread;

    protected:
        bool    _threadRunning;
        std::mutex  _mClient;
        std::condition_variable _clientNotifier;

    };
}

#endif //SERVERAUTH_BASESERVER_HH
