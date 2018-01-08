#include "ArgumentLoader/ArgumentLoader.hh"
#include "ArgumentLoader/Configure.hh"
#include "ServerRegister/ServerRegister.hh"

int main(int ac, char **av) {
    rtp::ArgumentLoader argumentLoader;

    if (!argumentLoader.loadArguments(ac, av)) {
        return 1;
    }
    rtp::Configure::configure(argumentLoader.get());
    rtp::ServerRegister serverRegister;

    return 0;
}