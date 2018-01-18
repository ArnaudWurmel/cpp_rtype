//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_BOOSTUDPINPUTMANAGER_HH
#define SERVERGAME_BOOSTUDPINPUTMANAGER_HH

# include <boost/asio.hpp>
#include <boost/bind.hpp>
# include "IUdpInputManager.hh"
# include "BoostSocketManager.hh"
# include "BoostUdpClient.hpp"

namespace NetworkAbstract {
    template<typename ClientCallback>
    class BoostUdpInputManager : public IUdpInputManager, public std::enable_shared_from_this<BoostUdpInputManager<ClientCallback> > {
    public:
        explicit BoostUdpInputManager(unsigned short port) : _port(port), _socket(BoostSocketManager::_ioService), _listeningEndpoint(boost::asio::ip::udp::v4(), _port) {
            _acceptClient = false;
        }

        void    initSocket(std::string const& authToken) override {
            _authToken = authToken;
            if (_socket.is_open()) {
                _socket.close();
            }
            _socket.open(boost::asio::ip::udp::v4());
            _socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            _socket.bind(_listeningEndpoint);
        }

        void    run() override {
            startSession();
        }

        void    stop() override {
            if (_socket.is_open()) {
                _socket.close();
            }
        }

        void    startAcceptingClient() override {
            _acceptClient = true;
        }

        void    stopAcceptingClient() override {
            _acceptClient = false;
        }

        void    handleNewData(NetworkAbstract::Message const& message, boost::asio::ip::udp::endpoint const& from) {
            auto    iterator = std::find_if(_acceptedClient.begin(), _acceptedClient.end(), [&](std::shared_ptr<BoostUdpClient<ClientCallback> > const& client) {
                return client->getEndpoint() == from;
            });
            if (iterator == _acceptedClient.end()) {
                if (_acceptClient) {
                    _acceptedClient.push_back(std::shared_ptr<BoostUdpClient<ClientCallback> >(new BoostUdpClient<ClientCallback>(std::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::authorizeWithToken, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2), from)));
                    iterator = _acceptedClient.end() - 1;
                }
                else {
                    return ;
                }
            }
            (*iterator)->injectInput(message);
            iterator = _acceptedClient.begin();
            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->hasTimedOut()) {
                    _acceptedClient.erase(iterator);
                }
                else {
                    ++iterator;
                }
            }
        }

        void    startSession() {
            _socket.async_receive_from(boost::asio::buffer(_readM.data(), Message::maxBodySize + Message::headerSize), _clientEndpoint,
                                        boost::bind(&NetworkAbstract::BoostUdpInputManager<ClientCallback>::handleReceiveData, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }

        void    handleReceiveData(const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                if (_readM.decodeHeader()) {
                    _readM.decodeData();
                    handleNewData(_readM, _clientEndpoint);
                    startSession();
                }
            }
        }

        bool    haveAcceptedClient() const {
            auto iterator = _acceptedClient.begin();

            while (iterator != _acceptedClient.end()) {
                if ((*iterator)->isAuthorized()) {
                    return true;
                }
                ++iterator;
            }
            return false;
        }

        ~BoostUdpInputManager() override {
            stop();
            BoostSocketManager::_ioService.stop();
        }

    private:
        bool    authorizeWithToken(std::shared_ptr<BoostUdpClient<ClientCallback> > registratedClient, std::string const& token) {
            if (_authToken.compare(token)) {

            }
            return false;
        }

    private:
        unsigned short  _port;
        boost::asio::ip::udp::socket    _socket;
        boost::asio::ip::udp::endpoint  _listeningEndpoint;
        boost::asio::ip::udp::endpoint  _clientEndpoint;

    private:
        std::string _authToken;
        bool    _acceptClient;
        std::vector<std::shared_ptr<BoostUdpClient<ClientCallback> > >   _acceptedClient;

    private:
        NetworkAbstract::Message    _readM;
    };
}


#endif //SERVERGAME_BOOSTUDPINPUTMANAGER_HH
