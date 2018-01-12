//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_AVIEWCONTROLLER_HH
#define CLIENTNETWORK_AVIEWCONTROLLER_HH

#include <vector>
#include "../DataGetter/DataGetter.hh"

namespace   rtp {
    class   AViewController {
    public:
        virtual ~AViewController() = default;

        virtual bool    render() = 0;
        virtual void    viewDidReappear() = 0;
        virtual std::vector<rtp::DataGetter>    getCommandObserver() const = 0;
    };
}

#endif //CLIENTNETWORK_AVIEWCONTROLLER_HH
