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
    class RTypeManager;
    class RegisteredServer;
    class GameServer;
    class ClientRegister;
    class RegisteredClient;

    template<typename T>
    class Logger {
    public:
        Logger() {
            say("Instanced");
        }

        virtual ~Logger() {
            say("Destructed");
        }

    protected:
        template<typename U = T>
        void    say(std::string const& message, bool withName = true, typename std::enable_if<std::is_same<U, RTypeManager>::value>::type* = 0) {
            LoggerHelper::lockOutput();
            if (withName) {
                std::cout << "[" << name() << "] <" << message << ">" << std::endl;
            }
            else {
                std::cout << message << std::endl;
            }
            LoggerHelper::unlockOutput();
        }

        template<typename U = T>
        void    say(std::string const& message, bool withName = true, typename std::enable_if<!std::is_same<U, RTypeManager>::value>::type* = 0) {
            if (LoggerHelper::isDebugEnabled()) {
                LoggerHelper::lockOutput();
                std::cerr << "[" << name() << "] <" << message << ">" << std::endl;
                LoggerHelper::unlockOutput();
            }
        }

    private:
        template<typename U = T>
        const char *name(typename std::enable_if<std::is_same<U, ServerRegister>::value>::type* = 0) {
            return "ServerRegister";
        }

        template<typename U = T>
        const char  *name(typename std::enable_if<std::is_same<U, RTypeManager>::value>::type* = 0) {
            return "RTypeManager";
        }

        template<typename U = T>
        const char   *name(typename std::enable_if<std::is_same<U, RegisteredServer>::value>::type* = 0) {
            return "RegisteredServer";
        }

        template<typename U = T>
        const char   *name(typename std::enable_if<std::is_same<U, GameServer>::value>::type* = 0) {
            return "GameServer";
        }

        template<typename U = T>
        const char   *name(typename std::enable_if<std::is_same<U, ClientRegister>::value>::type* = 0) {
            return "ClientRegister";
        }

        template<typename U = T>
        const char   *name(typename std::enable_if<std::is_same<U, RegisteredClient>::value>::type* = 0) {
            return "RegisteredClient";
        }
    };
}

#endif //SERVERAUTH_LOGGER_HPP
