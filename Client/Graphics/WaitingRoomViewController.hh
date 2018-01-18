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
        std::vector<rtp::DataGetter::Command>   getCommandObserver() const override;
        void    handleInput(NetworkAbstract::Message const&) override;

    private:
        void    handleNewOwner(NetworkAbstract::Message const&);
        void    handleStartMatchmaking(NetworkAbstract::Message const&);
        void    handleStopMatchmaking(NetworkAbstract::Message const&);
        void    handleServerFound(NetworkAbstract::Message const&);
        void    authorizedToPlay(std::shared_ptr<NetworkAbstract::ISocket> from, NetworkAbstract::Message const& response);

    private:
        void    createListPlayer();

    private:
        int _roomId;
        bool _isOwner;
        bool _onMatchmaking;
        bool _continue;

    private:
        RootViewController& _delegate;
        std::map<rtp::DataGetter::Command, std::function<void (NetworkAbstract::Message const&)> >  _functionPtrs;
        std::condition_variable _gameCv;
    };
}

#endif //CLIENTNETWORK_WAITINGROOMVIEWCONTROLLER_HH
