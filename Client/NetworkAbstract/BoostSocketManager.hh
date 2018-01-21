//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_BOOSTSOCKETMANAGER_HH
#define SERVERGAME_BOOSTSOCKETMANAGER_HH

# include <boost/asio.hpp>
# include "ISocketManager.hh"

namespace NetworkAbstract {
    class BoostSocketManager : public ISocketManager {
    public:
        BoostSocketManager();
        ~BoostSocketManager() override;

    public:
        std::shared_ptr<ISocket>    getSocket(SocketType, std::condition_variable&) override;
        void    enableSocket() override;
        void    disableSocket() override;
        bool    isEnabled() const override;

    public:
        static boost::asio::io_service _ioService;
    };
}

#endif //SERVERGAME_BOOSTSOCKETMANAGER_HH
