//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef CLIENTNETWORK_BOOSTUDPSERVER_HH
#define CLIENTNETWORK_BOOSTUDPSERVER_HH

# include <memory>
# include <thread>
# include <boost/asio.hpp>
# include <boost/bind.hpp>
# include "Message/Message.h"
# include "BoostUdpClient.hpp"
# include "ISocket.h"
# include "BoostSocket.h"

namespace NetworkAbstract {
    template<typename T>
    class BoostUdpServer : public IServer<T>, public std::enable_shared_from_this<BoostUdpServer<T> > {
    public:
        explicit BoostUdpServer(unsigned short port) : _endpoint(boost::asio::ip::udp::v4(), port), _socket(_ioService, _endpoint) {
            _acceptIncommingConnexion = false;
        }

        ~BoostUdpServer() {
            stop();
        }

    public:
        void    init(std::string const& authToken) {
            _acceptIncommingConnexion = false;
            _acceptedList.clear();
            _authToken = authToken;
        }

        void    acceptIncommingConnexion(bool state) {
            _acceptIncommingConnexion = state;
        }

        void    run() {
            startReceive();
            _threadRunner = std::unique_ptr<std::thread>(new std::thread( [&] {
                this->_ioService.run();
                std::cout << "Exiting..." << std::endl;
            }));
        }

        void    stop() {
            _ioService.stop();
            if (_threadRunner) {
                _threadRunner->join();
            }
        }

        std::shared_ptr<ISocket>    getEmptyASocket(std::condition_variable& awaker) {
            return std::shared_ptr<NetworkAbstract::ISocket>(new BoostSocket(_ioService, awaker));
        }

        std::vector<std::shared_ptr<BoostUdpClient<rtp::APlayer> > > const&    getClient() const {
            return _acceptedList;
        }

    private:
        void    newData(boost::asio::ip::udp::endpoint const& endpoint, NetworkAbstract::Message const& message) {
            auto iterator = std::find_if(_acceptedList.begin(), _acceptedList.end(), [&](std::shared_ptr<BoostUdpClient<T> > const& client) {
                return client->getEndpoint().address() == endpoint.address() && endpoint.port() == client->getEndpoint().port();
            });
            if (iterator == _acceptedList.end() && _acceptIncommingConnexion && _acceptedList.size() < 4) {
                if (!_authToken.empty()) {
                    _acceptedList.push_back(std::make_shared<BoostUdpClient<T> >(endpoint, _authToken));
                }
            }
            else if (iterator == _acceptedList.end()) {
                return ;
            }
            (*iterator)->injectInput(message);
        }

        void    handleReadHeader(const boost::system::error_code &error, std::size_t size) {
            if (!error && size == NetworkAbstract::Message::headerSize) {
                if (_readM.decodeHeader()) {
                    _socket.async_receive_from(boost::asio::buffer(_readM.getBody(), _readM.getBodySize()), _clientEndpoint,
                                               boost::bind(&BoostUdpServer::handleReadBody, this->shared_from_this(), boost::asio::placeholders::error,
                                                           boost::asio::placeholders::bytes_transferred));
                }
            }
        }

        void    handleReadBody(const boost::system::error_code& error, std::size_t size) {
            if (!error && size == NetworkAbstract::Message::headerSize) {
                newData(_clientEndpoint, _readM);
                startReceive();
            }
        }

        void    startReceive() {
            _socket.async_receive_from(
                    boost::asio::buffer(_readM.data(), NetworkAbstract::Message::headerSize), _clientEndpoint,
                    boost::bind(&BoostUdpServer::handleReadHeader, this->shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        }

    private:
        bool    _acceptIncommingConnexion;

    private:
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::endpoint  _endpoint;
        boost::asio::ip::udp::socket    _socket;
        std::vector<std::shared_ptr<BoostUdpClient<T> > > _acceptedList;
        std::unique_ptr<std::thread> _threadRunner;

    private:
        std::string _authToken;
        NetworkAbstract::Message    _readM;
        boost::asio::ip::udp::endpoint  _clientEndpoint;
    };
}

#endif //CLIENTNETWORK_BOOSTUDPSERVER_HH
