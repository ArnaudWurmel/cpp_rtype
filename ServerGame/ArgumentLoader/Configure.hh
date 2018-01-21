//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_CONFIGURE_HH
#define SERVERAUTH_CONFIGURE_HH

# include <unordered_map>
# include <functional>
# include <string>
# include "ArgumentLoader.hh"

namespace rtp {
    class Configure {
    public:
        static void configure(std::vector<ArgumentLoader::Argument> const&);

    private:
        Configure();
        ~Configure();

    private:
        void    makeConfiguration(std::vector<ArgumentLoader::Argument> const&);

    private:
        void    configureDebug(std::string const&);

    private:
        std::unordered_map<std::string, std::function<void (std::string const&)> >  _functionPtr;
    };
}

#endif //SERVERAUTH_CONFIGURE_HH
