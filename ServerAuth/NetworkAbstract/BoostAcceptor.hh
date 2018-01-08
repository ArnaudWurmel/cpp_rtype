//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_BOOSTACCEPTOR_HH
#define SERVERAUTH_BOOSTACCEPTOR_HH

# include <string>
# include <thread>
# include <condition_variable>
# include <mutex>
# include <boost/asio.hpp>
# include "IAcceptor.hh"
# include "BoostSocket.h"
# include "../Logger/Logger.hpp"

namespace NetworkAbstract {
    class BoostAcceptor : public IAcceptor {
    public:
        BoostAcceptor(unsigned short port, std::condition_variable&);
        ~BoostAcceptor() override;

    public:
        void    run() override;
        void    stop() override;
        void    startAccept() override;
        bool    haveAwaitingClient() override;
        std::shared_ptr<ISocket> acceptClient() override;

    private:
        void handle_accept(std::shared_ptr<BoostSocket>, const boost::system::error_code& error);

    private:
        boost::asio::ip::tcp::endpoint  _endpoint;
        boost::asio::io_service _ioService;
        boost::asio::ip::tcp::acceptor  _acceptor;
        std::queue<std::shared_ptr<BoostSocket> >    _clientQueue;
        std::mutex  _queueLocker;

    private:
        std::thread _threadRunner;

    private:
        std::mutex  _haveAction;
        std::condition_variable&    _notifier;
    };
}

#endif //SERVERAUTH_BOOSTACCEPTOR_HH
