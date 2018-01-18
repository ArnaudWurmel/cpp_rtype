//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_BOOSTUDPCLIENT_HPP
#define SERVERGAME_BOOSTUDPCLIENT_HPP

#include <boost/asio/ip/udp.hpp>
#include <memory>

namespace NetworkAbstract {
    template<typename T>
    class   BoostUdpClient : public T, public std::enable_shared_from_this<BoostUdpClient<T> > {
    public:
        using AuthorizationCallback = std::function<bool (std::shared_ptr<BoostUdpClient<T> >, std::string const&)>;

    public:
        explicit BoostUdpClient(AuthorizationCallback authorizationCallback, boost::asio::ip::udp::endpoint const& endpoint) : _endpoint(endpoint), _authorizationCallback(authorizationCallback) {}

        boost::asio::ip::udp::endpoint const&   getEndpoint() const {
            return _endpoint;
        }

        bool    injectInput(NetworkAbstract::Message const& message) {
            std::cout << "Here : " << message.getType() << std::endl;
            if (message.getType() == T::Command::AUTHORIZE) {
                std::cout << "Authorized Message" << std::endl;
                T::setAuthorize(_authorizationCallback(this->shared_from_this(), std::string(message.getBody(), message.getBodySize())));
            }
            else if (T::isAuthorized()) {
                T::injectInput(message);
            }
            return T::isAuthorized();
        }

        bool    hasTimedOut() const {
            return false;
        }

        ~BoostUdpClient() = default;

    private:
        boost::asio::ip::udp::endpoint  _endpoint;
        AuthorizationCallback   _authorizationCallback;
    };
}

#endif //SERVERGAME_BOOSTUDPCLIENT_HPP
