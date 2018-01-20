//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef SERVERGAME_LINUXDLLMANAGER_HH
#define SERVERGAME_LINUXDLLMANAGER_HH

# include "ADLLManager.hh"

namespace rtp {
    class LinuxDLLManager : public ADLLManager {
    public:
        explicit LinuxDLLManager(std::string const&);
        ~LinuxDLLManager() override;

    public:
        bool    init() override;
        AEnemy* getEnemyFromLib(unsigned int, int, int) override;

    private:
        std::string _libPath;
        void    *_handler;
        AEnemy  *(*_instancier)(unsigned int, int, int);

    private:
        static std::string  _ext;
    };
}


#endif //SERVERGAME_LINUXDLLMANAGER_HH
