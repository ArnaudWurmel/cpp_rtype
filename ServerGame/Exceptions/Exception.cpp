//
// Created by wurmel_a on 11/06/17.
//

#include "Exception.hh"

rtp::Exception::Exception(std::string const& what) {
    _what = what;
}

const char* rtp::Exception::what() const throw() {
    return _what.c_str();
}

rtp::Exception::~Exception() {}

rtp::NetworkException::NetworkException() : Exception("Network Error, connexion reset by peer") {}
rtp::NetworkException::~NetworkException() {}

rtp::ParsingNetworkException::ParsingNetworkException() : Exception("Network Error, ask for unknown file/content") {}
rtp::ParsingNetworkException::~ParsingNetworkException() {}

rtp::MissingFiles::MissingFiles() : Exception("Missing file for good execution...") {}
rtp::MissingFiles::~MissingFiles() {}