//
// Created by Arnaud WURMEL on 20/01/2018.
//

#ifndef _WIN32
#include <dlfcn.h>
#endif // !_WIN32
#include <iostream>
#include "LinuxDLLManager.hh"

#ifdef __APPLE__
std::string rtp::LinuxDLLManager::_ext = ".dylib";
#else
std::string rtp::LinuxDLLManager::_ext = ".so";
#endif

rtp::LinuxDLLManager::LinuxDLLManager(std::string const& libPath) : _libPath(libPath) {}

bool    rtp::LinuxDLLManager::init() {
#ifdef _WIN32
	return false;
#else
	std::string path = _libPath + ".dylib";
	char    *error;

	_handler = dlopen(path.c_str(), RTLD_LAZY);
	if (!_handler) {
		std::cout << "Can't instanciate monster <" << dlerror() << ">" << std::endl;
		return false;
	}
	_instancier = reinterpret_cast<AEnemy *(*)(unsigned int, int, int)>(dlsym(_handler, "instanciateMonster"));
	if ((error = dlerror()) != NULL) {
		std::cout << "Can't instanciate monster <" << error << ">" << std::endl;
		return false;
	}
	return true;
#endif // _WIN32
}

rtp::AEnemy *rtp::LinuxDLLManager::getEnemyFromLib(unsigned int id, int x, int y) {
    return _instancier(id, x, y);
}

rtp::LinuxDLLManager::~LinuxDLLManager() {
#ifndef _WIN32
	if (_handler) {
		dlclose(_handler);
	}
#endif // !_WIN32
}