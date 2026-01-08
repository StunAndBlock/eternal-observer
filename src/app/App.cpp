#include "app/App.hpp"

App::App(HINSTANCE hInstance) : uConfigurator_(L"observer.cfg"), uLauncher_(&uConfigurator_, &uConfig_, &uDPMWrapper_, &uNetChecker_){
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

void App::mainframe(){
    usecase::Launcher::STATUS code = uLauncher_.start();
    switch (code)
    {
    case usecase::Launcher::STATUS::OK:
        break;
    case usecase::Launcher::STATUS::WARNING:
        break;
    default:
        uLauncher_.fatalErrorNotification();
        PostThreadMessage(mainThread_,WM_QUIT, 0, 0);
        return;
    }
    return;
}

int App::start(){
    mainThread_ = GetCurrentThreadId();
    std::thread th(&App::mainframe, this);
    this->dispatch();
    if (th.joinable()) {
        th.join();
    }
    return 0;
}