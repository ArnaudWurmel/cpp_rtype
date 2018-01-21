//
// Created by Arnaud WURMEL on 09/01/2018.
//

#include "IAcceptor.hh"

std::vector<unsigned int>    NetworkAbstract::IAcceptor::_maxFd;
std::mutex  NetworkAbstract::IAcceptor::_lock;

unsigned int NetworkAbstract::IAcceptor::getMaxFd() {
#if _WIN32
    return 0;
#else
    _lock.lock();
    unsigned int max = 0;
    auto it = _maxFd.begin();

    while (it != _maxFd.end()) {
        if ((*it) > max) {
            max = *it;
        }
        ++it;
    }
    _lock.unlock();
    return max;
#endif
}

void    NetworkAbstract::IAcceptor::newFd(int fd) {
    _lock.lock();
    _maxFd.push_back(fd);
    _lock.unlock();
}

void    NetworkAbstract::IAcceptor::deleteFd(int fd) {
    _lock.lock();
    auto it = _maxFd.begin();

    while (it != _maxFd.end()) {
        if ((*it) == fd) {
            _maxFd.erase(it);
        }
        else {
            ++it;
        }
    }
    _lock.unlock();
}