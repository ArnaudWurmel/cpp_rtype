//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_SPAWNABLEENTITY_HH
#define SERVERGAME_SPAWNABLEENTITY_HH


namespace rtp {
    class SpawnableEntity {
    public:
        SpawnableEntity();
        ~SpawnableEntity();

    public:
        bool    isSpawned() const;
        void    spawn();

    private:
        bool    _isSpawned;
    };
}


#endif //SERVERGAME_SPAWNABLEENTITY_HH
