//
// Created by Arnaud WURMEL on 09/01/2018.
//

#ifndef SERVERAUTH_LINUXACCEPTOR_HH
#define SERVERAUTH_LINUXACCEPTOR_HH

# include <condition_variable>
# include <mutex>
#include <thread>
# include "IAcceptor.hh"
# include "LinuxSocket.hh"

namespace NetworkAbstract {
    class LinuxAcceptor : public IAcceptor {
    public:
        LinuxAcceptor(std::condition_variable&, unsigned short);
        ~LinuxAcceptor() override;

    public:
        bool startAccept() override;
        bool haveAwaitingClient() override;
        std::shared_ptr<NetworkAbstract::ISocket>   acceptClient() override;
        void    run() override;
        void    stop() override;
        bool    isRunning() override;

    private:
        void    acceptLoop();

    private:
        std::unique_ptr<NetworkAbstract::LinuxSocket>   _serverSocket;
        std::queue<std::shared_ptr<NetworkAbstract::LinuxSocket> >  _clientList;
        std::mutex  _clientListLocker;
        unsigned short  _port;

    private:
        std::condition_variable _clientAwake;
        std::condition_variable& _haveNewClient;
        std::mutex  _clientMutex;
        std::unique_ptr<std::thread> _threadAccept;
        bool _continueRunning;

    };
}


#endif //SERVERAUTH_LINUXACCEPTOR_HH
