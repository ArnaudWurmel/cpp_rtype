//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef CLIENTNETWORK_AENTITY_HH
#define CLIENTNETWORK_AENTITY_HH

# include <memory>
# include "ADrawableEntity.hh"

namespace rtp {
    class AEntity : public rtp::ADrawableEntity {
    public:
        static std::shared_ptr<AEntity> instanciateFromToken(std::vector<std::string> const&);

    private:
        AEntity(int entityId, int rotation, int x, int y, int currentFrame, std::string const& spriteName);

    public:
        ~AEntity() override;

    private:

    };
}


#endif //CLIENTNETWORK_AENTITY_HH
