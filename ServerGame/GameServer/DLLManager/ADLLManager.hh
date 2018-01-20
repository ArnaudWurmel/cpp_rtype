//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_ADLLMANAGER_HH
#define SERVERGAME_ADLLMANAGER_HH

# include <memory>
# include "../Entities/AEnemy.hh"

namespace   rtp {
    class   ADLLManager {
    public:
        virtual ~ADLLManager() = default;

    public:
        virtual bool    init() = 0;
        virtual AEnemy* getEnemyFromLib(int, int) = 0;

    public:
        static std::shared_ptr<ADLLManager> get(std::string const&);
    };
}

#endif //SERVERGAME_ADLLMANAGER_HH
