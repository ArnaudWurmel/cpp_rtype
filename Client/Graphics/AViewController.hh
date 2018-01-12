//
// Created by Arnaud WURMEL on 11/01/2018.
//

#ifndef CLIENTNETWORK_AVIEWCONTROLLER_HH
#define CLIENTNETWORK_AVIEWCONTROLLER_HH

namespace   rtp {
    class   AViewController {
    public:
        virtual ~AViewController() = default;

        virtual bool    render() = 0;
        
    };
}

#endif //CLIENTNETWORK_AVIEWCONTROLLER_HH
