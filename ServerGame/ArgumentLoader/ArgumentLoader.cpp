//
// Created by wurmel_a on 1/8/18.
//

#include <algorithm>
#include <iostream>
#include "ArgumentLoader.hh"

rtp::ArgumentLoader::Argument::Argument(std::string const& argument, bool haveSupplementValue) : argument(argument), haveSupplementValue(haveSupplementValue) {
    value = "";
}

rtp::ArgumentLoader::Argument::~Argument() = default;

rtp::ArgumentLoader::ArgumentLoader() {}

void    rtp::ArgumentLoader::enableArgument(std::string const& argument, bool haveSupplementValue) {
    _argumentList.push_back(Argument(argument, haveSupplementValue));
}

bool    rtp::ArgumentLoader::loadArguments(int ac, char **av) {
    int i = 1;

    while (i < ac) {
        auto itemFounded = std::find_if(_argumentList.begin(), _argumentList.end(),
        [&](Argument const& argument) {
            return !argument.argument.compare(av[i]);
        });
        if (itemFounded == _argumentList.end()) {
            std::cerr << "Unknown argument : `" << av[i] << "`" << std::endl;
            return false;
        }
        rtp::ArgumentLoader::Argument   argument((*itemFounded).argument, (*itemFounded).haveSupplementValue);

        if (argument.haveSupplementValue && i + 1 < ac) {
            argument.value = av[++i];
        }
        else if (i + 1 >= ac && argument.haveSupplementValue) {
            std::cerr << "Missing parameters for : `" << argument.argument << "`" << std::endl;
            return false;
        }
        _argumentFoundedList.push_back(argument);
        ++i;
    }
    return true;
}

std::vector<rtp::ArgumentLoader::Argument> const&   rtp::ArgumentLoader::get() const {
    return _argumentFoundedList;
}

bool    rtp::ArgumentLoader::haveArg(std::string const &argName) const {
    auto itemFounded = std::find_if(_argumentFoundedList.begin(), _argumentFoundedList.end(),
                                    [&](Argument const& argument) {
                                        return argument.argument == argName;
                                    });
    return itemFounded != _argumentFoundedList.end();
}

rtp::ArgumentLoader::Argument const&  rtp::ArgumentLoader::getArg(std::string const& argKey) const {
    auto itemFounded = std::find_if(_argumentFoundedList.begin(), _argumentFoundedList.end(),
                                    [&](Argument const& argument) {
                                        return argument.argument == argKey;
                                    });
    return *itemFounded;
}

rtp::ArgumentLoader::~ArgumentLoader() {}