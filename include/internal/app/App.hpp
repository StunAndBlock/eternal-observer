#ifndef APP_APP_HPP_
#define APP_APP_HPP_
#include "usecase/Launcher.hpp"
#include "usecase/config/Configurator.hpp"
class App {
    public:
        App(HINSTANCE);
        int init();
        int start();
    private:
        void dispatch();
        HINSTANCE hInstance_;
        usecase::Configurator uConfigurator_;
        usecase::Config uConfig_;
        usecase::Launcher uLauncher_;
        

};







#endif //!APP_APP_HPP_