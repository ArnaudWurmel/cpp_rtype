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
        void    listClient(std::vector<std::string> const&);
        void    exitServer(std::vector<std::string> const&);
        void    help(std::vector<std::string> const&);

    private:
        std::vector<std::string>    getTokenFrom(std::string const&);

    private:
        bool    _continue;
        std::shared_ptr<ServerRegister> _serverRegister;
        std::unordered_map<std::string, std::function<void (std::vector<std::string> const&)> >    _functionPtr;
    };
}

#endif //SERVERAUTH_RTYPEMANAGER_HH
