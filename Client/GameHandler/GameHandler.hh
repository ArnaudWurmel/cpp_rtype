//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef CLIENTNETWORK_GAMEHANDLER_HH
#define CLIENTNETWORK_GAMEHANDLER_HH

namespace rtp {
    class GameHandler {
    public:
        enum    Command {
            AUTHORIZE = 0,
            SPAWN_PLAYER = 1
        };
    };
}

#endif //CLIENTNETWORK_GAMEHANDLER_HH
