//
// Created by wurmel_a on 1/8/18.
//

#include "LoggerHelper.hh"

bool rtp::LoggerHelper::_debugEnabled = false;

void rtp::LoggerHelper::setDebug(bool value) {
    rtp::LoggerHelper::_debugEnabled = value;
}

bool rtp::LoggerHelper::isDebugEnabled() {
    return _debugEnabled;
}