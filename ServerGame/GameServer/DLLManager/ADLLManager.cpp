//
// Created by Arnaud WURMEL on 20/01/2018.
//

#include "ADLLManager.hh"

#ifndef _WIN32
    #include "LinuxDLLManager.hh"
#endif

std::shared_ptr<rtp::ADLLManager> rtp::ADLLManager::get(std::string const& path) {
#ifndef _WIN32
    return std::shared_ptr<rtp::ADLLManager>(new LinuxDLLManager(path));
#endif
}