//
// Created by wurmel_a on 1/8/18.
//

#include "LoggerHelper.hh"

std::mutex rtp::LoggerHelper::_outputLocker;

bool rtp::LoggerHelper::_debugEnabled = false;

void rtp::LoggerHelper::setDebug(bool value) {
    rtp::LoggerHelper::_debugEnabled = value;
}

bool rtp::LoggerHelper::isDebugEnabled() {
    return _debugEnabled;
}

void rtp::LoggerHelper::lockOutput() {
    _outputLocker.lock();
}

void rtp::LoggerHelper::unlockOutput() {
    _outputLocker.unlock();
}