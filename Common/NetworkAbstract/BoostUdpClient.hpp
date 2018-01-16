//
// Created by Arnaud WURMEL on 14/01/2018.
//

#ifndef SERVERGAME_BOOSTPLAYER_HH
#define SERVERGAME_BOOSTPLAYER_HH

# include <boost/asio.hpp>

namespace NetworkAbstract {
    template<typename T>
    class BoostUdpClient : public T {
    public:
        using   OnAuthorizationGranted = std::function<void (bool, boost::asio::ip::udp::endpoint)>;

    public:
        explicit BoostUdpClient(OnAuthorizationGranted const& onAuthorizationGranted, boost::asio::ip::udp::endpoint const& endpoint, std::string const& authToken) : T(std::bind(&NetworkAbstract::BoostUdpClient<T>::onAuthorization, this, std::placeholders::_1), authToken) {
            _endpoint = endpoint;
            _onAuthorizationGranted = onAuthorizationGranted;
            _lastMessage = std::chrono::system_clock::now();
        }

        ~BoostUdpClient() = default;

    public:
        boost::asio::ip::udp::endpoint const&   getEndpoint() const {
            return _endpoint;
        }

        bool    haveTimedOut() const {
            return _lastMessage.time_since_epoch().count() + 10000000 < std::chrono::system_clock::now().time_since_epoch().count();
        }

        bool    injectInput(NetworkAbstract::Message const& message) {
            _lastMessage = std::chrono::system_clock::now();
            return T::injectInput(message);
        }

    private:
        void    onAuthorization(bool success) {
            _onAuthorizationGranted(success, _endpoint);
        }

    private:
        boost::asio::ip::udp::endpoint  _endpoint;
        OnAuthorizationGranted _onAuthorizationGranted;
        std::chrono::time_point<std::chrono::system_clock> _lastMessage;
    };
}

#endif //SERVERGAME_BOOSTPLAYER_HH
