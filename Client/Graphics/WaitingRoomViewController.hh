//
// Created by Arnaud WURMEL on 12/01/2018.
//

#ifndef CLIENTNETWORK_WAITINGROOMVIEWCONTROLLER_HH
#define CLIENTNETWORK_WAITINGROOMVIEWCONTROLLER_HH

# include "AViewController.hh"
# include "RootViewController.hh"

namespace   rtp {
    class WaitingRoomViewController : public AViewController {
    public:
        WaitingRoomViewController(RootViewController&, int roomId, bool isOwner = false);
        ~WaitingRoomViewController() override;

    public:
        bool    render() override;
        void    viewDidReappear() override;

    private:
        int _roomId;
        bool _isOwner;

    private:
        RootViewController& _delegate;
    };
}

#endif //CLIENTNETWORK_WAITINGROOMVIEWCONTROLLER_HH
