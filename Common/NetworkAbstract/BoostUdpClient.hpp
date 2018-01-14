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
        explicit BoostUdpClient(boost::asio::ip::udp::endpoint const& endpoint, std::string const& authToken) : T(authToken) {
            _endpoint = endpoint;
        }

        ~BoostUdpClient() = default;

    public:
        boost::asio::ip::udp::endpoint const&   getEndpoint() const {
            return _endpoint;
        }

    private:
        boost::asio::ip::udp::endpoint  _endpoint;
    };
}

#endif //SERVERGAME_BOOSTPLAYER_HH
