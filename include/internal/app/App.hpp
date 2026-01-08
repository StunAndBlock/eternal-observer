#ifndef APP_APP_HPP_
#define APP_APP_HPP_
#include "usecase/launcher/Launcher.hpp"
#include "usecase/config/Configurator.hpp"
#include "usecase/core/DPMWrapper.hpp"
class App {
    public:
        App(HINSTANCE);
        int init();
        int start();
    private:
        void dispatch();
        void mainframe();
        HINSTANCE hInstance_;
        usecase::Configurator uConfigurator_;
        usecase::Config uConfig_;
        usecase::Launcher uLauncher_;
        usecase::DPMWrapper uDPMWrapper_;
        usecase::NetChecker uNetChecker_;
        DWORD mainThread_;
        

};







#endif //!APP_APP_HPP_