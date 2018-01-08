//
// Created by Arnaud WURMEL on 08/01/2018.
//

#ifndef SERVERAUTH_RTYPEMANAGER_HH
#define SERVERAUTH_RTYPEMANAGER_HH

# include <memory>
# include <functional>
# include <unordered_map>
# include "../ServerRegister/ServerRegister.hh"
# include "../Logger/Logger.hpp"

namespace rtp {
    class RTypeManager : private Logger<RTypeManager> {
    public:
        RTypeManager(std::shared_ptr<ServerRegister>&);
        ~RTypeManager();

    public:
        bool loop();

    private:
        void    listClient();
        void    exitServer();

    private:
        bool    _continue;
        std::shared_ptr<ServerRegister> _serverRegister;
        std::unordered_map<std::string, std::function<void ()> >    _functionPtr;
    };
}

#endif //SERVERAUTH_RTYPEMANAGER_HH
