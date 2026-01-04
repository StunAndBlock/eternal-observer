#ifndef USECASE_LAUNCHER_HPP_
#define USECASE_LAUNCHER_HPP_

#include "ui/Launcher.hpp"
#include "usecase/config/Configurator.hpp"
#include <condition_variable>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
namespace usecase  {
    class Launcher {
        public:
            enum STATUS : int {
                OK,
                ERR
            };
            Launcher(usecase::Configurator*, usecase::Config*);
            STATUS init(HINSTANCE);
            STATUS start();
            void testNotification();
        private:
            ui::Launcher launcherUi_;
            usecase::Configurator* configurator_;
            usecase::Config* config_;
    };
}
#endif //!USECASE_LAUNCHER_HPP_