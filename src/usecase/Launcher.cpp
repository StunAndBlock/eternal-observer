#include "usecase/Launcher.hpp"


namespace usecase {

    Launcher::Launcher(usecase::Configurator* configurator, usecase::Config* config){
        configurator_ = configurator;
        config_ = config;
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
    Launcher::STATUS Launcher::start(){
        int code = 0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        launcherUi_.set_stage(ui::Launcher::STAGE_LEVEL::REGISTER_CONFIGURATION);
        code = configurator_->load(config_->getModifiableConfig(),config_->getRepresentation());
        switch (code) {
            case Configurator::STATUS::FIRST_START:
                launcherUi_.testNotification();
            case Configurator::STATUS::OK:
                break;
            default:
                return STATUS::ERR;
        }
        // usecase::config& data = config_->getModifiableConfig();
        // data.observerCoreCmd_= L"test";
        // configurator_->save(data,config_->getRepresentation());
        
        launcherUi_.set_stage(ui::Launcher::STAGE_LEVEL::PREPARING_CORE);
        std::this_thread::sleep_for(std::chrono::milliseconds(3600));
        launcherUi_.set_stage(ui::Launcher::STAGE_LEVEL::NET_WARMUP);
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        launcherUi_.set_stage(ui::Launcher::STAGE_LEVEL::FINISHING);
        std::this_thread::sleep_for(std::chrono::milliseconds(1400));
        launcherUi_.set_stage(ui::Launcher::STAGE_LEVEL::COMPLETED);
        return STATUS::OK;
    }


}