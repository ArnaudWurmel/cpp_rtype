//
// Created by Arnaud WURMEL on 12/01/2018.
//

#ifndef CLIENTNETWORK_ROOMLISTVIEWCONTROLLER_HH
#define CLIENTNETWORK_ROOMLISTVIEWCONTROLLER_HH

# include "AViewController.hh"
# include "RootViewController.hh"

namespace rtp {
    class RoomListViewController : public AViewController {
    public:
        explicit RoomListViewController(RootViewController&);
        ~RoomListViewController() override;

    public:
        bool    render() override;

    private:
        RootViewController& _delegate;
    };
}

#endif //CLIENTNETWORK_ROOMLISTVIEWCONTROLLER_HH
