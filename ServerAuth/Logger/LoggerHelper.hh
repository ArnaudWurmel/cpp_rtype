//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_LOGGERHELPER_HH
#define SERVERAUTH_LOGGERHELPER_HH

namespace rtp {
    class LoggerHelper {
    public:
        static bool isDebugEnabled();
        static void setDebug(bool);

    private:
        static bool _debugEnabled;
    };
}


#endif //SERVERAUTH_LOGGERHELPER_HH
