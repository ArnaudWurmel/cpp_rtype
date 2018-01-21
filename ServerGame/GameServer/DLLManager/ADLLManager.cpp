//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "ADLLManager.hh"

#ifdef _WIN32
	#include "WindowsDLLManager.hh"
#else
    #include "LinuxDLLManager.hh"
#endif

std::shared_ptr<rtp::ADLLManager> rtp::ADLLManager::get(std::string const& path) {
#ifdef _WIN32
	return std::shared_ptr<rtp::ADLLManager>(new WindowsDLLManager(path));
#else
    return std::shared_ptr<rtp::ADLLManager>(new LinuxDLLManager(path));
#endif
}