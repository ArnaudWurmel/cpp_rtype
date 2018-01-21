//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef CLIENTNETWORK_BOOSTSOCKETUDP_HH
#define CLIENTNETWORK_BOOSTSOCKETUDP_HH

# include <boost/asio.hpp>
# include <memory>
# include "ISocket.h"

namespace NetworkAbstract {
    class BoostUdpSocket : public ISocket, public std::enable_shared_from_this<BoostUdpSocket> {
    public:
        BoostUdpSocket(boost::asio::io_service&, std::condition_variable&);
        ~BoostUdpSocket() override;

    public:
        void    close() override;
        bool    isOpen() const override;
        bool    connectSocket(std::string const&, unsigned short) override;
        void    startSession() override;
        std::string getIpAddr() const override;
        void    write(NetworkAbstract::Message) override;

    private:
        void    handleRead(const boost::system::error_code& error, std::size_t);
        void    handleWrite(boost::system::error_code const&, std::size_t);

    private:
        boost::asio::ip::udp::socket    _socket;
        Message _readM;
        std::mutex  _mutex;
        std::queue<NetworkAbstract::Message>  _writeList;
        boost::asio::ip::udp::endpoint  _clientEndpoint;
        boost::asio::ip::udp::endpoint  _serverEndpoint;
    };
}

#endif //CLIENTNETWORK_BoostUdpSocketUDP_HH
