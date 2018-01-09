//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_ARGUMENTLOADER_HH
#define SERVERAUTH_ARGUMENTLOADER_HH

# include <string>
# include <vector>

namespace rtp {
    class ArgumentLoader {
    public:
        struct  Argument {
            bool    haveSupplementValue;
            std::string value;
            std::string argument;

            Argument(std::string const&, bool);
            ~Argument();
        };
    public:
        ArgumentLoader();
        ~ArgumentLoader();

    public:
        bool    loadArguments(int, char **);
        std::vector<Argument> const&    get() const;

    public:
        bool    haveArg(std::string const& argName) const;
        Argument const& getArg(std::string const&) const;

    private:
        std::vector<Argument>   _argumentList;
        std::vector<Argument>   _argumentFoundedList;
    };
}

#endif //SERVERAUTH_ARGUMENTLOADER_HH
