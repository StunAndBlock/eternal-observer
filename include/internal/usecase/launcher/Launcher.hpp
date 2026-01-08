#ifndef USECASE_LAUNCHER_LAUNCHER_HPP_
#define USECASE_LAUNCHER_LAUNCHER_HPP_

#include "ui/Launcher.hpp"
#include "usecase/launcher/Interface.hpp"
#include "usecase/config/Configurator.hpp"
#include "usecase/core/DPMWrapper.hpp"
#include "usecase/net/NetChecker.hpp"
#include <condition_variable>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
namespace usecase  {
    class Launcher : public ILauncher {
        public:
            Launcher(usecase::Configurator*, usecase::Config*, usecase::DPMWrapper*, usecase::NetChecker*);
            STATUS init(HINSTANCE) override;
            STATUS start() override ;
            void testNotification();
            void fatalErrorNotification();
        private:
            ui::Launcher launcherUi_;
            usecase::Configurator* configurator_;
            usecase::Config* config_;
            usecase::DPMWrapper* wrapper_;
            usecase::NetChecker* netChecker_;

    };
}
#endif //!USECASE_LAUNCHER_LAUNCHER_HPP_