//
// Created by wurmel_a on 11/06/17.
//

#ifndef CPP_INDIE_STUDIO_EXCEPTION_HH
#define CPP_INDIE_STUDIO_EXCEPTION_HH

# include <exception>
# include <string>

namespace rtp {
    class Exception : public std::exception {
    public:
        Exception(std::string const&);
        ~Exception();

    public:
        virtual char const* what() const throw();

    private:
        std::string _what;
    };

    class NetworkException : public Exception {
    public:
        NetworkException();
        ~NetworkException();
    };

    class ParsingNetworkException : public Exception {
    public:
        ParsingNetworkException();
        ~ParsingNetworkException();
    };

    class   MissingFiles : public Exception {
    public:
        MissingFiles();
        ~MissingFiles();
    };
}

#endif //CPP_INDIE_STUDIO_EXCEPTION_HH
