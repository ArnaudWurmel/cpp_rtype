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
        static std::shared_ptr<Player>  instanciateFromInfo(std::string const&);

    private:
        Player(int id, std::string const& pseudo, std::string const& spriteName, int x, int y, int currentFrame);

    public:
        ~Player() override;

    public:
        int getId() const;

    public:
        void    updateFrom(std::vector<std::string> const&);

    private:
        int _id;
    };
}

#endif //CLIENTNETWORK_PLAYER_HH
