//
// Created by wurmel_a on 1/8/18.
//

#ifndef SERVERAUTH_LOGGERHELPER_HH
#define SERVERAUTH_LOGGERHELPER_HH

# include <mutex>

namespace rtp {
    class LoggerHelper {
    public:
        static bool isDebugEnabled();
        static void setDebug(bool);

        static void lockOutput();
        static void unlockOutput();

    private:
        static bool _debugEnabled;
        static std::mutex   _outputLocker;
    };
}


#endif //SERVERAUTH_LOGGERHELPER_HH
