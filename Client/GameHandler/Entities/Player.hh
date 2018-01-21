//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef CLIENTNETWORK_PLAYER_HH
#define CLIENTNETWORK_PLAYER_HH

# include <memory>
# include "ADrawableEntity.hh"

namespace rtp {
    class Player : public ADrawableEntity {
    public:
        static std::shared_ptr<Player>  instanciateFromInfo(std::string const&, bool me = false);

    private:
        Player(int id, std::string const& pseudo, std::string const& spriteName, int x, int y, int currentFrame, bool me);

    public:
        ~Player() override;

    public:
        int getId() const;

    public:
        void    updateFrom(std::vector<std::string> const&);
        bool    isMe() const;

    private:
        int _id;
        bool    _me;
    };
}

#endif //CLIENTNETWORK_PLAYER_HH
