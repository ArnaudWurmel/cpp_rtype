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
        enum    Command {
            AUTHORIZE = 0,
            SPAWN_PLAYER = 1
        };

    public:
        explicit BoostUdpServer(unsigned short port) : _endpoint(boost::asio::ip::udp::v4(), port), _socket(_ioService) {
            _acceptIncommingConnexion = false;
            boost::system::error_code ec;
            init("");
            std::cout << port << std::endl;
        }

        ~BoostUdpServer() {
            stop();
            _ioService.stop();
            if (_threadRunner) {
                _threadRunner->join();
            }
        }

    public:
        void    init(std::string const& authToken) {
            if (!_socket.is_open()) {
                _socket.open(boost::asio::ip::udp::v4());
                boost::system::error_code ec;
                _socket.set_option(boost::asio::socket_base::reuse_address(true), ec);
                _socket.bind(_endpoint);
            }
            _acceptIncommingConnexion = false;
            _acceptedList.clear();
            _authToken = authToken;
            if (authToken.size() > 0) {
                startReceive();
            }
        }

        void    acceptIncommingConnexion(bool state) {
            _acceptIncommingConnexion = state;
        }

        void    run() {
            std::cout << "Runned" << std::endl;
            startReceive();
            _threadRunner = std::unique_ptr<std::thread>(new std::thread( [&] {
                this->_ioService.run();
                std::cout << "Exiting..." << std::endl;
            }));
        }

        void    stop() {
            if (_socket.is_open()) {
                close();
            }
        }

        void    close() {
            _socket.cancel();
            _socket.close();
        }

        std::shared_ptr<ISocket>    getEmptyASocket(std::condition_variable& awaker) {
            return std::shared_ptr<NetworkAbstract::ISocket>(new BoostSocket(_ioService, awaker));
        }

        std::vector<std::shared_ptr<T> > const&    getClient() const {
            return _acceptedList;
        }

        std::vector<std::shared_ptr<T> >&    getClient() {
            return _acceptedList;
        }

    private:
        void    newData(boost::asio::ip::udp::endpoint const& endpoint, NetworkAbstract::Message const& message) {
            std::cout << "New input" << std::endl;
            auto iterator = std::find_if(_acceptedList.begin(), _acceptedList.end(), [&](std::shared_ptr<T> const& client) {
                return client->getEndpoint().address() == endpoint.address() && endpoint.port() == client->getEndpoint().port();
            });
            std::cout << "Authorized to be accepted : " << _acceptIncommingConnexion << std::endl;
            if (iterator == _acceptedList.end() && _acceptIncommingConnexion && _acceptedList.size() < 4) {
                if (!_authToken.empty()) {
                    _acceptedList.push_back(std::make_shared<T>(std::bind(&NetworkAbstract::BoostUdpServer<T>::clientAccessGame, this, std::placeholders::_1, std::placeholders::_2), endpoint, _authToken));
                    iterator = _acceptedList.end() - 1;
                }
            }
            else if (iterator == _acceptedList.end() || !_acceptIncommingConnexion) {
                return ;
            }
            (*iterator)->injectInput(message);
        }

        void    clientAccessGame(bool success, boost::asio::ip::udp::endpoint const& clientEndpoint) {
            std::cout << "Player called" << std::endl;
            auto    newPlayerIterator = std::find_if(_acceptedList.begin(), _acceptedList.end(), [&] (std::shared_ptr<T> const& player) {
                return player->getEndpoint() == clientEndpoint;
            });

            if (newPlayerIterator == _acceptedList.end()) {
                std::cout << "Can't find a player" << std::endl;
                return ;
            }

            std::cout << "Before send" << std::endl;
            Message authorizedMessage;

            authorizedMessage.setType(AUTHORIZE);
            if (success) {
                std::string body;

                *((*newPlayerIterator).get()) >> body;
                authorizedMessage.setBody(body.c_str(), body.length());
            }
            else {
                authorizedMessage.setBody(std::string("failure").c_str(), std::string("failure").length());
            }

            std::cout << "Before write" << std::endl;
            writeToClient(clientEndpoint, authorizedMessage);
            std::cout << "Send to client " << success << std::endl;
            std::string spawnBody;
            Message spawnMessage;

            spawnMessage.setType(SPAWN_PLAYER);
            *((*newPlayerIterator).get()) >> spawnBody;
            spawnMessage.setBody(spawnBody.c_str(), spawnBody.length());
            auto iterator = _acceptedList.begin();

            while (iterator != _acceptedList.end()) {
                if ((*iterator)->isAuthorized() && (*iterator)->getEndpoint() != clientEndpoint) {
                    writeToClient((*iterator)->getEndpoint(), spawnMessage);
                }
                ++iterator;
            }
        }

        void    writeToClient(std::shared_ptr<T> const& client, NetworkAbstract::Message const& message) {
            this->writeToClient(client->getEndpoint(), message);
        }

        void    writeToClient(boost::asio::ip::udp::endpoint const& clientEndpoint, NetworkAbstract::Message const& message) {
            _writingLocker.lock();
            bool    onWriting = !_writingList.empty();

            _writingList.push(std::make_pair(clientEndpoint, message));
            _writingList.back().second.encodeHeader();
            _writingList.back().second.encodeData();
            if (!onWriting) {
                _socket.async_send_to(boost::asio::buffer(_writingList.front().second.data(), _writingList.front().second.totalSize()),
                                        _writingList.front().first, boost::bind(&NetworkAbstract::BoostUdpServer<T>::handleWrited, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
                _writingLocker.unlock();
                return ;
            }
            _writingLocker.unlock();
        }

        void    handleWrited(boost::system::error_code const& error, std::size_t writed) {
            std::cout << error << std::endl;
            _writingLocker.lock();
            _writingList.pop();
            if (!_writingList.empty()) {
                _socket.async_send_to(boost::asio::buffer(_writingList.front().second.data(), _writingList.front().second.totalSize()),
                                      _writingList.front().first, boost::bind(&NetworkAbstract::BoostUdpServer<T>::handleWrited, this->shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

            }
            _writingLocker.unlock();
        }

        void    handleReadHeader(const boost::system::error_code &error, std::size_t size) {
            std::cout << error.message() << " " << size << " " << Message::headerSize << std::endl;
            if (!error) {
                if (_readM.decodeHeader()) {
                    _readM.decodeData();
                    newData(_clientEndpoint, _readM);
                }
                startReceive();
            }
        }

        void    startReceive() {
            std::cout << "startReceive()" << std::endl;
            _socket.async_receive_from(
                    boost::asio::buffer(_readM.data(), NetworkAbstract::Message::maxBodySize + NetworkAbstract::Message::headerSize), _clientEndpoint,
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
        std::vector<std::shared_ptr<T> > _acceptedList;
        std::unique_ptr<std::thread> _threadRunner;

    private:
        std::string _authToken;
        NetworkAbstract::Message    _readM;
        boost::asio::ip::udp::endpoint  _clientEndpoint;

    private:
        std::queue<std::pair<boost::asio::ip::udp::endpoint, NetworkAbstract::Message> > _writingList;
        std::mutex  _writingLocker;
    };
}

#endif //CLIENTNETWORK_BOOSTUDPSERVER_HH