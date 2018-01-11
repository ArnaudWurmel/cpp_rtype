#include <iostream>
#include "ArgumentLoader/ArgumentLoader.hh"
#include "ArgumentLoader/Configure.hh"
#include "DataGetter/DataGetter.hh"
#include "Graphics/RootViewController.hh"

int main(int ac, char **av) {
    rtp::ArgumentLoader argumentLoader;
    rtp::RootViewController rootViewController;

    argumentLoader.enableArgument("-d", true);
    if (!argumentLoader.loadArguments(ac, av)) {
        return 1;
    }
    rtp::Configure::configure(argumentLoader.get());
    rootViewController.loop();
    /*try {
        if (!connectClient(argumentLoader, dataGetter)) {
            return 1;
        }
    }
    catch (std::exception& exception) {
        std::cerr << "Exception : " << exception.what() << std::endl;
        return 1;
    }*/
    return 0;
}