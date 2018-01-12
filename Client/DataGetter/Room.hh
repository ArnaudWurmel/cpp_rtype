//
// Created by Arnaud WURMEL on 12/01/2018.
//

#ifndef CLIENTNETWORK_ROOM_HH
#define CLIENTNETWORK_ROOM_HH

# include <string>
# include <vector>

namespace rtp {
    class Room {
    public:
        Room();
        ~Room();

    public:
        bool    init(std::string const&);
        int    getId() const;
        std::vector<std::string> const&    getPlayerList() const;

    private:
        int    _id;
        std::vector<std::string> _playerList;
    };
}

#endif //CLIENTNETWORK_ROOM_HH
