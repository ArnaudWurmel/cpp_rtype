//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef SERVERGAME_BOOSTUDPSOCKET_HH
#define SERVERGAME_BOOSTUDPSOCKET_HH


# include <mutex>
# include <condition_variable>
# include <boost/asio.hpp>
# include <memory>
# include "ISocket.h"
# include "Message/Message.h"


namespace NetworkAbstract {
    class BoostUdpSocket : public ISocket, public std::enable_shared_from_this<BoostUdpSocket> {
    public:
        BoostUdpSocket(boost::asio::io_service &, std::condition_variable &);

        ~BoostUdpSocket();

    public:
        void close() override;
        bool isOpen() const override;
        bool connectSocket(std::string const &, unsigned short) override;
        void startSession() override;
        std::string getIpAddr() const override;
        void write(NetworkAbstract::Message) override;

    private:
        void handleReadHeader(const boost::system::error_code &error, std::size_t);
        void handleWrite(boost::system::error_code const &, std::size_t);

    private:
        boost::asio::ip::udp::socket _socket;
        Message _readM;
        std::mutex _mutex;
        std::queue<NetworkAbstract::Message> _writeList;
        boost::asio::ip::udp::endpoint  _serverEndpoint;
        boost::asio::ip::udp::endpoint  _readedEndpoint;
        boost::asio::ip::udp::resolver  _resolver;
    };
}

#endif //SERVERGAME_BOOSTUDPSOCKET_HH
