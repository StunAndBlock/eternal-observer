#ifndef USECASE_LAUNCHER_INTERFACE_HPP_
#define USECASE_LAUNCHER_INTERFACE_HPP_

#include <condition_variable>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <windows.h>
namespace usecase  {
    class ILauncher {
        public:
        enum STATUS : int {
            OK,
            WARNING,
            ERR
        };
        enum class STAGE_LEVEL {
            STARTING,
            REGISTER_CONFIGURATION,
            PREPARING_CORE,
            NET_WARMUP,
            FINISHING,
            COMPLETED
        };
        virtual STATUS init(HINSTANCE) = 0;
        virtual STATUS start() = 0;
    };

}
#endif //!USECASE_LAUNCHERINTERFACE_HPP_