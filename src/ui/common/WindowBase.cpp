#include "ui/common/WindowBase.hpp"
#include "ui/common/Errors.hpp"


LRESULT __stdcall WindowBase::ex_ambitu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    WindowBase* pThis = nullptr;
    if (msg == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pThis = reinterpret_cast<WindowBase*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowBase::window_proc));
        pThis->wnd_ = hwnd;
        return window_proc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
LRESULT __stdcall WindowBase::window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowBase* pThis = reinterpret_cast<WindowBase*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (pThis) {
        return pThis->handler(msg, wParam,lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT __stdcall WindowBase::handler(UINT msg, WPARAM wParam, LPARAM lParam){
    return DefWindowProc(this->wnd_, msg, wParam, lParam);
}

WindowBase::STATUS WindowBase::create(WNDCLASSW wc, 
    LPCWSTR wName, 
    DWORD style, 
    const RECT& dimParam,     
    HWND hPd){
    wc.lpfnWndProc = this->ex_ambitu;
    ATOM res = RegisterClass(&wc);
    if (!res) {
        ui::discoverError();
        STATUS::ERR;
    }
    wnd_ = CreateWindowEx(0,
        wc.lpszClassName,
        wName,
        style,
        dimParam.left, dimParam.top, dimParam.right,dimParam.bottom,
        hPd,
        NULL,
        wc.hInstance,
        this
    );
    if (!wnd_) {
        ui::discoverError();
        STATUS::ERR;
    }
    return STATUS::OK;
}