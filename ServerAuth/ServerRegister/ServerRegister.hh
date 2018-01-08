//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_SERVERREGISTER_HH
#define SERVERAUTH_SERVERREGISTER_HH

# include <string>
# include "../Logger/Logger.hpp"
# include "../ArgumentLoader/ArgumentLoader.hh"

namespace rtp {
    class ServerRegister : private Logger<ServerRegister> {

    public:
        explicit ServerRegister(std::string const& host = "0.0.0.0", unsigned short port = 8080);
        ~ServerRegister();
    };
}

#endif //SERVERAUTH_SERVERREGISTER_HH
