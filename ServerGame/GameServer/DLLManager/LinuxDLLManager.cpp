//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <dlfcn.h>
#include <iostream>
#include "LinuxDLLManager.hh"

#ifdef __APPLE__
std::string rtp::LinuxDLLManager::_ext = ".dylib";
#else
std::string rtp::LinuxDLLManager::_ext = ".so";
#endif

rtp::LinuxDLLManager::LinuxDLLManager(std::string const& libPath) : _libPath(libPath) {}

bool    rtp::LinuxDLLManager::init() {
    std::string path = _libPath + ".dylib";
    char    *error;

    _handler = dlopen(path.c_str(), RTLD_LAZY);
    if (!_handler) {
        std::cout << "Can't instanciate monster <" << dlerror() << ">" << std::endl;
        return false;
    }
    _instancier = reinterpret_cast<AEnemy *(*)(int, int)>(dlsym(_handler, "instanciateMonster"));
    if ((error = dlerror()) != NULL) {
        std::cout << "Can't instanciate monster <" << error << ">" << std::endl;
        return false;
    }
    return true;
}

rtp::AEnemy *rtp::LinuxDLLManager::getEnemyFromLib(int x, int y) {
    return _instancier(x, y);
}

rtp::LinuxDLLManager::~LinuxDLLManager() {
    if (_handler) {
        dlclose(_handler);
    }
}