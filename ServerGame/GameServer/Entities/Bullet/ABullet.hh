//
// Created by Arnaud WURMEL on 19/01/2018.
//

#ifndef SERVERGAME_ABULLET_HH
#define SERVERGAME_ABULLET_HH

# include "../AEntity.hh"
# include "../Vector2.hh"
# include "../SpawnableEntity.hh"

namespace rtp {
    class ABullet : public rtp::AEntity, public rtp::SpawnableEntity {
    public:
        enum    BulletType {
            Ally = 0,
            Monster = 1
        };

    public:
        ABullet(BulletType, Vector2<int> position, Vector2<int> direction);
        ~ABullet() override;

    public:
        virtual void    onCollide(APlayer&) = 0;

    public:
        void    update(double) override;
        bool    isExpectedToBeDeleted() const override;

    public:
        static std::string  getSpriteFromBulletType(BulletType);

    private:
        Vector2<int>    _direction;
        BulletType  _type;
        bool    _alive;
    };
}

#endif //SERVERGAME_ABULLET_HH
