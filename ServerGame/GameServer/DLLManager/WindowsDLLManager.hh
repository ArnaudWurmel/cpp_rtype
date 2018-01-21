//
// Created by Victorien FISCHER on 21/01/2018.
//

#ifndef SERVERGAME_WINDOWSDLLMANAGER_HH
#define SERVERGAME_WINDOWSDLLMANAGER_HH

#ifdef _WIN32
#include <Windows.h>
#endif // !_WIN32
# include "ADLLManager.hh"

namespace rtp {
    class WindowsDLLManager : public ADLLManager {
    public:
        explicit WindowsDLLManager(std::string const&);
        ~WindowsDLLManager() override;

    public:
        bool    init() override;
        AEnemy* getEnemyFromLib(unsigned int, int, int) override;

    private:
        std::string _libPath;
#ifdef _WIN32
		HMODULE	_handler;
#else
		void    *_handler;
#endif // _WIN32
        AEnemy  *(*_instancier)(unsigned int, int, int);

    private:
        static std::string  _ext;
    };
}


#endif //SERVERGAME_WINDOWSDLLMANAGER_HH
