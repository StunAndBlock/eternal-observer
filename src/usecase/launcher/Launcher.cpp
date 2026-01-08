#include "usecase/launcher/Launcher.hpp"


namespace usecase {

    Launcher::Launcher(usecase::Configurator* configurator, usecase::Config* config, 
        usecase::DPMWrapper* wrapper,  usecase::NetChecker* netChecker){
        configurator_ = configurator;
        config_ = config;
        wrapper_ = wrapper;
        netChecker_ = netChecker;
    }
    Launcher::STATUS Launcher::init(HINSTANCE hInst){
        int code = 0;    
        WNDCLASSW wc = { 0 };
        wc.hInstance = hInst;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = L"Launcher";
        RECT rect = {400,400,400,800};
        code = launcherUi_.create(wc, L"Observer Launcher", (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX ), rect);
        if (code != ui::Launcher::STATUS::OK){
            return STATUS::ERR;
        }
        return STATUS::OK;
    }
    void Launcher::testNotification(){
        launcherUi_.testNotification();
    }
    void Launcher::fatalErrorNotification(){
        launcherUi_.fatalErrorNotification();
    }
    Launcher::STATUS Launcher::start(){
        int code = 0;
        STATUS result = STATUS::OK;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        launcherUi_.set_stage(ILauncher::STAGE_LEVEL::REGISTER_CONFIGURATION);
        code = configurator_->load(config_->getModifiableConfig(),config_->getRepresentation());
        switch (code) {
            case Configurator::STATUS::FIRST_START:
                launcherUi_.testNotification();
                result = STATUS::WARNING;
                break;
            case Configurator::STATUS::OK:
                launcherUi_.set_stage(ILauncher::STAGE_LEVEL::PREPARING_CORE);
                wrapper_->restruct(config_->getConfig().dpmWrapperPath, config_->getConfig().dpmWrapperArguments);
                break;
            default:
                return STATUS::ERR;
        }
        launcherUi_.set_stage(ILauncher::STAGE_LEVEL::NET_WARMUP);
        auto res = netChecker_->check();
        code = res.second;
        switch (code) {
            case NetChecker::STATUS::OK:
                break;
            case NetChecker::STATUS::NOT_EVEN_CONNECTED:
                result = STATUS::WARNING;
                break;
            case NetChecker::STATUS::NO_INET_ACCESS:
                result = STATUS::WARNING;
                break;
            default:
                result = STATUS::WARNING;
                break;
        }
        launcherUi_.set_stage(ILauncher::STAGE_LEVEL::FINISHING);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        launcherUi_.set_stage(ILauncher::STAGE_LEVEL::COMPLETED);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); 
        return result;
    }


}