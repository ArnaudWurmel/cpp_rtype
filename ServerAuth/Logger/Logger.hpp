//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_LOGGER_HPP
#define SERVERAUTH_LOGGER_HPP

# include <iostream>
# include <string>
# include <type_traits>
# include "LoggerHelper.hh"

namespace rtp {

    class ServerRegister;

    template<typename T>
    class Logger {
    public:
        Logger() {
            say("Instanced");
        }

        ~Logger() {
            say("Destructed");
        }

    protected:
        void    say(std::string const& message) {
            if (LoggerHelper::isDebugEnabled()) {
                std::cout << "[" << name() << "] <" << message << ">" << std::endl;
            }
        }

    private:
        template<typename U = T>
        const char *name(typename std::enable_if<std::is_same<U, ServerRegister>::value>::type* = 0) {
            return "ServerRegister";
        }
    };
}

#endif //SERVERAUTH_LOGGER_HPP
