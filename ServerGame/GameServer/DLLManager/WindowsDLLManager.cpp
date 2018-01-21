//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include <iostream>
#include "WindowsDLLManager.hh"

//std::string rtp::WindowsDLLManager::_ext = ".dll";

rtp::WindowsDLLManager::WindowsDLLManager(std::string const& libPath) : _libPath(libPath) {}

bool    rtp::WindowsDLLManager::init() {
#ifdef _WIN32
	std::string path = _libPath + ".dll";
	DWORD	error;

	_handler = LoadLibrary(path.c_str());
	std::cout << "Loading " << path.c_str() << std::endl;
	if (!_handler) {
		std::cout << "Can't instanciate monster <" << GetLastError() << ">" << std::endl;
		return false;
	}
	_instancier = reinterpret_cast<AEnemy *(*)(unsigned int, int, int)>(GetProcAddress(_handler, "instanciateMonster"));
	if ((error = GetLastError()) != NULL) {
		std::cout << "Can't instanciate monster <" << error << ">" << std::endl;
		return false;
	}
	return true;
#else
	return false;
#endif // _WIN32
}

rtp::AEnemy *rtp::WindowsDLLManager::getEnemyFromLib(unsigned int id, int x, int y) {
    return _instancier(id, x, y);
}

rtp::WindowsDLLManager::~WindowsDLLManager() {
#ifdef _WIN32
	if (_handler) {
		FreeLibrary(_handler);
	}
#endif // !_WIN32
}