//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_LOGINVIEWCONTROLLER_HH
#define CLIENTNETWORK_LOGINVIEWCONTROLLER_HH

# include "AViewController.hh"
# include "RootViewController.hh"

namespace rtp {
    class LoginViewController : public AViewController {
    public:
        explicit LoginViewController(RootViewController&);
        ~LoginViewController() override;

    public:
        bool    render() override;

    private:
        RootViewController& _rootViewController;
        bool    _continue;

    private:
        char    _name[255];
        char    _host[255];
    };
}

#endif //CLIENTNETWORK_LOGINVIEWCONTROLLER_HH
