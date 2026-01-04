#include "ui/common/Errors.hpp"
void ui::discoverError(){
    DWORD dwError = GetLastError();
    LPVOID lpMsgBuf;
    if (dwError == ERROR_SUCCESS){
        LPCWSTR msg = L"something went wrong";
        int len = lstrlenW(msg) + 1;
        lpMsgBuf = new WCHAR[len];
        memcpy(lpMsgBuf,msg, len*sizeof(WCHAR));
    } else {
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);
    }
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONERROR);
    LocalFree(lpMsgBuf);
}