//
// Created by Arnaud WURMEL on 18/01/2018.
//

#ifndef SERVERGAME_IUDPINPUTMANAGER_HH
#define SERVERGAME_IUDPINPUTMANAGER_HH

# include <memory>
# include <vector>

namespace NetworkAbstract {
    class   IUdpInputManager {
    public:
        virtual ~IUdpInputManager() = default;

        virtual void    initSocket(std::string const&) = 0;
        virtual void    run() = 0;
        virtual void    stop() = 0;
        virtual void    startAcceptingClient() = 0;
        virtual void    stopAcceptingClient() = 0;
        virtual bool    haveAcceptedClient() = 0;
        virtual void    updateAllPlayer(double) = 0;
        virtual void    sendUpdate() = 0;
    };
}

#endif //SERVERGAME_IUDPINPUTMANAGER_HH
