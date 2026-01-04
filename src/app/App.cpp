#include "app/App.hpp"

App::App(HINSTANCE hInstance) : uConfigurator_(L"observer.cfg"), uLauncher_(&uConfigurator_, &uConfig_){
    hInstance_ = hInstance;
}

int App::init(){
    INITCOMMONCONTROLSEX icc = { sizeof(icc) };
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);
    uLauncher_.init(hInstance_);
    return 0;
}


void App::dispatch(){
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int App::start(){


    std::thread th(&usecase::Launcher::start, &uLauncher_);
    this->dispatch();
    if (th.joinable()) {
        th.join();
    }
    return 0;
}