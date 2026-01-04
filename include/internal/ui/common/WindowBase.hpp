#ifndef UI_COMMON_WINDOWBASE_HPP_
#define UI_COMMON_WINDOWBASE_HPP_

#include <windows.h>

#include <commctrl.h>
class WindowBase {
    public:
        enum STATUS : int {
                OK,
                ERR
        };
        WindowBase() = default;
        ~WindowBase() = default;
        virtual STATUS create(WNDCLASSW, LPCWSTR, DWORD, const RECT&, HWND  = nullptr);
    protected:
        static LRESULT __stdcall ex_ambitu(HWND, UINT, WPARAM, LPARAM);
        static LRESULT __stdcall window_proc(HWND, UINT , WPARAM , LPARAM );
        virtual LRESULT __stdcall handler(UINT, WPARAM, LPARAM);
        
        HWND wnd_;
};






#endif //!UI_COMMON_WINDOWBASE_HPP_
