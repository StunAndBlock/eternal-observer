// main.cpp
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "app/App.hpp"
#pragma comment(lib, "comctl32.lib")
#define WM_TRAYICON (WM_USER + 1)


enum EyeState {
    EYE_CLOSED,
    EYE_OPENING,
    EYE_OPEN,
    EYE_CLOSING
};

EyeState g_eyeState = EYE_CLOSED;
int g_eyeProgress = 0; // 0..100
#define WM_SET_WINDOW_TEXT WM_USER + 100
//void CenterWindow(HWND hWnd, int width, int height);
DWORD style   = WS_POPUP;
DWORD exStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
#define ID_TRAY_START  1001
#define ID_TRAY_STOP   1002
#define ID_TRAY_EXIT   1003
#define ID_TRAY_SETTINGS   1004
// Your exact command, untouched
const wchar_t* ZAPRET_COMMAND = 
L".\\dpi\\winws.exe --wf-tcp=80,443,2053,2083,2087,2096,8443,12 --wf-udp=443,19294-19344,50000-50100,12 "
L"--filter-udp=443 --hostlist=\".\\dpi\\lists\\list-general.txt\" --hostlist-exclude=\".\\dpi\\lists\\list-exclude.txt\" --ipset-exclude=\".\\dpi\\lists\\ipset-exclude.txt\" --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fake-quic=\".\\dpi\\bin\\quic_initial_www_google_com.bin\" --new "
L"--filter-udp=19294-19344,50000-50100 --filter-l7=discord,stun --dpi-desync=fake --dpi-desync-fake-discord=\".\\dpi\\bin\\quic_initial_www_google_com.bin\" --dpi-desync-fake-stun=\".\\dpi\\bin\\quic_initial_www_google_com.bin\" --dpi-desync-repeats=6 --new "
L"--filter-tcp=2053,2083,2087,2096,8443 --hostlist-domains=discord.media --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fooling=ts --dpi-desync-fake-tls=\".\\dpi\\bin\\tls_clienthello_max_ru.bin\" --new "
L"--filter-tcp=443 --hostlist=\".\\dpi\\lists\\list-google.txt\" --ip-id=zero --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fooling=ts --dpi-desync-fake-tls=\".\\dpi\\bin\\tls_clienthello_www_google_com.bin\" --new "
L"--filter-tcp=80,443 --hostlist=\".\\dpi\\lists\\list-general.txt\" --hostlist-exclude=\".\\dpi\\lists\\list-exclude.txt\" --ipset-exclude=\".\\dpi\\lists\\ipset-exclude.txt\" --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fooling=ts --dpi-desync-fake-tls=\".\\dpi\\bin\\tls_clienthello_max_ru.bin\" --new "
L"--filter-udp=443 --ipset=\".\\dpi\\lists\\ipset-all.txt\" --hostlist-exclude=\".\\dpi\\lists\\list-exclude.txt\" --ipset-exclude=\".\\dpi\\lists\\ipset-exclude.txt\" --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fake-quic=\".\\dpi\\bin\\quic_initial_www_google_com.bin\" --new "
L"--filter-tcp=80,443,12 --ipset=\".\\dpi\\lists\\ipset-all.txt\" --hostlist-exclude=\".\\dpi\\lists\\list-exclude.txt\" --ipset-exclude=\".\\dpi\\lists\\ipset-exclude.txt\" --dpi-desync=fake --dpi-desync-repeats=6 --dpi-desync-fooling=ts --dpi-desync-fake-tls=\".\\dpi\\bin\\tls_clienthello_max_ru.bin\" --new "
L"--filter-udp=12 --ipset=\".\\dpi\\lists\\ipset-all.txt\" --ipset-exclude=\".\\dpi\\lists\\ipset-exclude.txt\" --dpi-desync=fake --dpi-desync-autottl=2 --dpi-desync-repeats=12 --dpi-desync-any-protocol=1 --dpi-desync-fake-unknown-udp=\".\\dpi\\bin\\quic_initial_www_google_com.bin\" --dpi-desync-cutoff=n3";

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LaunchHiddenProcess(const wchar_t* exePath);
void SetWindowOpacity(HWND hWnd, BYTE opacity);
void AddToTray(HWND hWnd);
void RemoveFromTray(HWND hWnd);
void StopWinws();
LRESULT __stdcall PP(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSettings(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSettingsProcButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSettingsCmdButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndSettingsOthersButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndSettingsCmdTab(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndSettingsOthersTab(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall StartupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
NOTIFYICONDATA nid = { 0 };
HANDLE g_hProcess = NULL;
HWND g_hMainWnd = NULL;
HWND hDpiSettingsButton = NULL;
HWND hDpiSettings = NULL;
HWND hDpiSettingsCmdTab = NULL;
HWND hDpiSettingsOthers = NULL;
HWND hDpiSettingsCmdTabButton = NULL;
HWND hDpiSettingsOthersButton = NULL;
HWND hStartup = NULL;
const wchar_t* WINDOW_CLASS = L"HiddenLauncherClass";
const wchar_t* DPI_SETTINGS = L"DPI_SETTINGS";
const wchar_t* DPI_SETTINGS_BUTTON = L"DPI_SETTINGS_BUTTON";
const wchar_t* DPI_SETTINGS_CMD_TAB = L"DPI_SETTINGS_CMD_TAB";
const wchar_t* DPI_SETTINGS_SETTINGS_OTHERS = L"DPI_SETTINGS_OTHERS";
const wchar_t* DPI_SETTINGS_CMD_TAB_BUTTON = L"DPI_SETTINGS_CMD_TAB_BUTTON";
const wchar_t* DPI_SETTINGS_SETTINGS_OTHERS_BUTTON = L"DPI_SETTINGS_OTHERS_BUTTON";
const wchar_t* STARTUP = L"STARTUP";



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   PWSTR pCmdLine, int nCmdShow) {
    App app(hInstance);
    app.init();
    app.start();

    // Register window class
    // WNDCLASSW wc = { 0 };
    // wc.lpfnWndProc = WndProc;
    // wc.hInstance = hInstance;
    // wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    // wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // wc.lpszClassName = WINDOW_CLASS;


    // RegisterClassW(&wc);
    
    // // Create main window (hidden initially)
    // g_hMainWnd = CreateWindowW(
    //     WINDOW_CLASS, L"Hidden Process Launcher",
    //     WS_OVERLAPPEDWINDOW,
    //     0, 0, 400, 300,
    //     NULL, NULL, hInstance, NULL
    // );
    
    // if (!g_hMainWnd) return 0;
    
    // // Make window semi-transparent (optional)
    // SetWindowOpacity(g_hMainWnd, 200);
    
    // // Add to system tray (optional)
    // AddToTray(g_hMainWnd); 
    // wc.lpfnWndProc = WndSettings;
    // wc.lpszClassName = DPI_SETTINGS;
    // RegisterClassW(&wc);
    // hDpiSettings =  CreateWindowW(
    //     DPI_SETTINGS, L"Dpi settings",
    //     WS_OVERLAPPEDWINDOW,
    //     0, 0, 500, 500,
    //     NULL, NULL, hInstance, NULL
    // );
    // wc.lpfnWndProc = WndSettingsCmdTab;
    // wc.lpszClassName = DPI_SETTINGS_CMD_TAB;
    // RegisterClassW(&wc);
    // hDpiSettingsCmdTab =  CreateWindowW(
    //     DPI_SETTINGS_CMD_TAB, L"Dpi settings cmd",
    //     WS_CHILDWINDOW,
    //     0, 50, 100, 100,
    //     hDpiSettings, NULL, hInstance, NULL
    // );
    // wc.lpfnWndProc = WndSettingsOthersTab;
    // wc.lpszClassName = DPI_SETTINGS_SETTINGS_OTHERS;
    // RegisterClassW(&wc);
    // hDpiSettingsOthers =  CreateWindowW(
    //     DPI_SETTINGS_SETTINGS_OTHERS, L"Dpi settings others",
    //     WS_CHILDWINDOW,
    //     0, 50, 100, 100,
    //     hDpiSettings, NULL, hInstance, NULL
    // );
    // wc.lpfnWndProc = WndSettingsCmdButton;
    // wc.lpszClassName = DPI_SETTINGS_CMD_TAB_BUTTON;
    // RegisterClassW(&wc);
    // hDpiSettingsCmdTabButton = CreateWindowW(
    //     DPI_SETTINGS_CMD_TAB_BUTTON, L"Dpi settings cmd tab button",
    //     WS_CHILDWINDOW,
    //     0, 0, 40, 40,
    //     hDpiSettings, NULL, hInstance, NULL
    // );

    // wc.lpfnWndProc = WndSettingsOthersButton;
    // wc.lpszClassName = DPI_SETTINGS_SETTINGS_OTHERS_BUTTON;
    // RegisterClassW(&wc);
    // hDpiSettingsOthersButton = CreateWindowW(
    //     DPI_SETTINGS_SETTINGS_OTHERS_BUTTON, L"Dpi settings others button",
    //     WS_CHILDWINDOW,
    //     50, 0, 40, 40,
    //     hDpiSettings, NULL, hInstance, NULL
    // );
    // wc.lpfnWndProc = WndSettingsProcButton;
    // wc.lpszClassName = DPI_SETTINGS_BUTTON;
    // wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    // RegisterClassW(&wc);
    // hDpiSettingsButton = CreateWindowW(
    //     DPI_SETTINGS_BUTTON, L"Dpi settings button",
    //     WS_CHILD,
    //     340, 220, 40, 40,
    //     g_hMainWnd, NULL, hInstance, NULL
    // );
    // wc.lpfnWndProc = StartupProc;
    // wc.lpszClassName = STARTUP;
    // wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
    // RegisterClassW(&wc);
    // hDpiSettingsButton = CreateWindowW(
    //     STARTUP, L"Startup",
    //     WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
    //     400, 400, 400, 800,
    //     NULL, NULL, hInstance, NULL
    // );

    // // Message loop    
    // std::thread th(startup);
    // MSG msg = { 0 };
    // while (GetMessage(&msg, NULL, 0, 0)) {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }


    
    return 0;
}
// LRESULT __stdcall StartupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
//     switch (msg)
//     {
//     case WM_CREATE:
//         SetWindowOpacity(hWnd, 200);
//         ShowWindow(hWnd, SW_SHOW);
//     break;
//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);

//         RECT rc;
//         GetClientRect(hWnd, &rc);

//         // Background
//         HBRUSH bg = CreateSolidBrush(RGB(15, 15, 20));
//         FillRect(hdc, &rc, bg);
//         SetTextColor(hdc, RGB(255, 255, 255));     // Red text
//         SetBkColor(hdc, RGB(255, 255, 0));     // Yellow background
//         SetBkMode(hdc, OPAQUE);               // Opaque background
//         DeleteObject(bg);
//         RECT rect = {0, 0, 400, 800};
//         DrawText(hdc, L"LDLP Loader initialized", 
//                     -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    
//          EndPaint(hWnd, &ps);
//     }
//     break;
//     case WM_SET_WINDOW_TEXT: {
//         std::wstring* pText = (std::wstring*)lParam;
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);
//         delete pText; // Clean up the allocated string
//         break;
//     }
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }

// LRESULT __stdcall PP(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
//     switch (msg)
//     {
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }

// LRESULT __stdcall WndSettings(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
//     switch (msg)
//     {
//     case WM_CREATE:
        
//         break;
//      case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);

//         RECT rc;
//         GetClientRect(hWnd, &rc);

//         // Background
//         HBRUSH bg = CreateSolidBrush(RGB(15, 15, 20));
//         FillRect(hdc, &rc, bg);
//         DeleteObject(bg);
//     }

//     break;    
//      case WM_SHOWWINDOW:
//         ShowWindow(hDpiSettingsCmdTabButton, SW_SHOW);
//         ShowWindow(hDpiSettingsOthersButton, SW_SHOW);
//         break;
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }

// LRESULT __stdcall WndSettingsCmdTab(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
//     switch (msg)
//     {
//     case WM_CREATE:
//         break;
//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);

//         RECT rc;
//         GetClientRect(hWnd, &rc);

//         // Background
//         HBRUSH bg = CreateSolidBrush(RGB(255, 0, 0));
//         FillRect(hdc, &rc, bg);
//         DeleteObject(bg);
//     }
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }
// LRESULT __stdcall WndSettingsOthersTab(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
//     switch (msg)
//     {
//     case WM_CREATE:
//         break;
//         case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);

//         RECT rc;
//         GetClientRect(hWnd, &rc);

//         // Background
//         HBRUSH bg = CreateSolidBrush(RGB(0, 255, 0));
//         FillRect(hdc, &rc, bg);
//         DeleteObject(bg);
//     }
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }
// LRESULT CALLBACK WndSettingsCmdButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//     switch (msg)
//     {
//     case WM_LBUTTONDOWN:
//         ShowWindow(hDpiSettingsCmdTab, SW_SHOW);
//         break;
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }
// LRESULT CALLBACK WndSettingsOthersButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//     switch (msg)
//     {
//     case WM_LBUTTONDOWN:
//         ShowWindow(hDpiSettingsOthers, SW_SHOW);
//         break;
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }

// LRESULT CALLBACK WndSettingsProcButton(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//     switch (msg)
//     {
//     case WM_LBUTTONDOWN:
//         ShowWindow(hDpiSettings, SW_SHOW);
//         break;
//     case WM_CREATE:
//         ShowWindow(hWnd, SW_SHOW);
//         break;
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return DefWindowProc(hWnd, msg, wParam, lParam);
// }
// LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//     switch (msg) {
//         case WM_TIMER:
//     if (g_eyeState == EYE_OPENING) {
//         g_eyeProgress += 4;
//         if (g_eyeProgress >= 100) {
//             g_eyeProgress = 100;
//             g_eyeState = EYE_OPEN;
//         }
//         InvalidateRect(hWnd, NULL, TRUE);
//     }
//     else if (g_eyeState == EYE_CLOSING) {
//         g_eyeProgress -= 4;
//         if (g_eyeProgress <= 0) {
//             g_eyeProgress = 0;
//             g_eyeState = EYE_CLOSED;
//         }
//         InvalidateRect(hWnd, NULL, TRUE);
//     }
//     break;
//     case WM_CREATE: {
//          SetTimer(hWnd, 1, 16, NULL);       
//         break;
//     }
//     case WM_TRAYICON:
//     if (lParam == WM_RBUTTONUP) {
//         POINT pt;
//         GetCursorPos(&pt);

//         HMENU hMenu = CreatePopupMenu();

//         if (!g_hProcess)
//             InsertMenuW(hMenu, -1, MF_BYPOSITION, ID_TRAY_START, L"Start winws");
//         else
//             InsertMenuW(hMenu, -1, MF_BYPOSITION, ID_TRAY_STOP, L"Stop winws");

//         InsertMenuW(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
//         InsertMenuW(hMenu, -1, MF_BYPOSITION, ID_TRAY_EXIT, L"Exit");

//         SetForegroundWindow(hWnd);
//         TrackPopupMenu(
//             hMenu,
//             TPM_RIGHTBUTTON | TPM_BOTTOMALIGN,
//             pt.x, pt.y, 0, hWnd, NULL
//         );

//         DestroyMenu(hMenu);
//     } else if (lParam == WM_LBUTTONUP){
//         ShowWindow(g_hMainWnd, SW_SHOW);
//         //CenterWindow(g_hMainWnd, 400, 300);

//     }
//     break;

//     case WM_COMMAND:
//     switch (LOWORD(wParam)) {
//     case 1: // Start
//          ShowWindow(g_hMainWnd, SW_SHOW);
//     //CenterWindow(g_hMainWnd, 400, 300);
//     g_eyeState = EYE_OPENING;
//         g_eyeState = EYE_OPENING;
//         break;

//     case 2: // Stop
//         StopWinws();
//         g_eyeState = EYE_CLOSING;
//         break;
//     case ID_TRAY_START: // tray menu Start
//         ShowWindow(g_hMainWnd, SW_SHOW);
//         //CenterWindow(g_hMainWnd, 400, 300);
//         g_eyeState = EYE_OPENING;
//         LaunchHiddenProcess(ZAPRET_COMMAND);
//         break;
//     case ID_TRAY_STOP: // tray menu Stop
//         g_eyeState = EYE_CLOSING;
//         StopWinws();
//         break;
//     case ID_TRAY_EXIT: // tray menu Exit
//         PostMessage(hWnd, WM_CLOSE, 0, 0); // triggers cleanup
//         break;
//     case ID_TRAY_SETTINGS:
//         ShowWindow(hDpiSettings, SW_SHOW);
//         //CenterWindow(hDpiSettings, 400, 300);
//         break;
//     }
    
//     break;
    
//     case WM_SYSCOMMAND: {
//         // Hide instead of minimize
//         if (wParam == SC_MINIMIZE) {
//             ShowWindow(hWnd, SW_HIDE);
//             return 0;
//         }
//         break;
//     }
//     case WM_KILLFOCUS:
//     ShowWindow(hWnd, SW_HIDE);
//     break;
//     case WM_DESTROY:
//      KillTimer(hWnd, 1);
//             RemoveFromTray(hWnd);
//             StopWinws();
//             PostQuitMessage(0);
//         break;
//     case WM_PAINT:
// {
//     PAINTSTRUCT ps;
//     HDC hdc = BeginPaint(hWnd, &ps);

//     RECT rc;
//     GetClientRect(hWnd, &rc);

//     // Background
//     HBRUSH bg = CreateSolidBrush(RGB(15, 15, 20));
//     FillRect(hdc, &rc, bg);
//     DeleteObject(bg);

//     // Eye center
//     int cx = (rc.right - rc.left) / 2;
//     int cy = 60;

//     int openness = g_eyeProgress; // 0..100
//     int height = openness / 3;    // scale

//     HPEN pen = CreatePen(PS_SOLID, 3, RGB(200, 200, 220));
//     SelectObject(hdc, pen);
//     SelectObject(hdc, GetStockObject(NULL_BRUSH));

//     if (openness > 0) {
//         // upper arc
//         Arc(hdc,
//             cx - 60, cy - height,
//             cx + 60, cy + height,
//             cx - 60, cy,
//             cx + 60, cy);

//         // lower arc
//         Arc(hdc,
//             cx - 60, cy - height,
//             cx + 60, cy + height,
//             cx + 60, cy,
//             cx - 60, cy);
//     }
//     else {
//         // closed eye (||)
//         MoveToEx(hdc, cx - 20, cy - 10, NULL);
//         LineTo(hdc, cx - 20, cy + 10);

//         MoveToEx(hdc, cx + 20, cy - 10, NULL);
//         LineTo(hdc, cx + 20, cy + 10);
//     }

//     DeleteObject(pen);
//     EndPaint(hWnd, &ps);
//     return 0;
// }
//     default:
//         return DefWindowProc(hWnd, msg, wParam, lParam);
//     }
//     return 0;
// }





// void LaunchHiddenProcess(const wchar_t* exePath) {
//      if (g_hProcess) return; 
//     STARTUPINFOW si = { sizeof(si) };
//     PROCESS_INFORMATION pi = { 0 };
    
//     // Configure to hide the console window
//     si.dwFlags = STARTF_USESHOWWINDOW;
//     si.wShowWindow = SW_HIDE;
//     std::wstring cmd = exePath;
//     // Create the process

//     if (!CreateProcessW(
//         L".\\dpi\\winws.exe",                   // No module name
//         cmd.data(),        // Command line
//         NULL,                   // Process security
//         NULL,                   // Thread security
//         FALSE,                  // No handle inheritance
//        0,       // Don't attach to parent console
//         NULL,                   // Environment
//         NULL,                   // Current directory
//         &si,
//         &pi
//     )) {
//       DWORD err = GetLastError();
//         wchar_t buf[256];
//         swprintf_s(buf, L"CreateProcess failed. GetLastError = %lu", err);
//         MessageBoxW(NULL, buf, L"Error", MB_ICONERROR);
//         return;
//     }

//         g_hProcess = pi.hProcess;
//         CloseHandle(pi.hThread);
// }

// void SetWindowOpacity(HWND hWnd, BYTE opacity) {
//     SetWindowLongW(hWnd, GWL_EXSTYLE, 
//         GetWindowLongW(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
//     SetLayeredWindowAttributes(hWnd, 0, opacity, LWA_ALPHA);
// }
// void StopWinws() {
//     if (!g_hProcess) return;

//     TerminateProcess(g_hProcess, 0); // winws has no graceful exit
//     CloseHandle(g_hProcess);
//     g_hProcess = NULL;
// }
// void AddToTray(HWND hWnd) {
//     nid.cbSize = sizeof(NOTIFYICONDATA);
//     nid.hWnd = hWnd;
//     nid.uID = 1;
//     nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//     nid.uCallbackMessage = WM_USER + 1;
//     nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//     wcscpy_s(nid.szTip, L"Hidden Launcher");
    
//     Shell_NotifyIcon(NIM_ADD, &nid);
// }

// void RemoveFromTray(HWND hWnd) {
//     Shell_NotifyIcon(NIM_DELETE, &nid);
// }

// void CenterWindow(HWND hWnd, int width, int height) {
//     int screenW = GetSystemMetrics(SM_CXSCREEN);
//     int screenH = GetSystemMetrics(SM_CYSCREEN);

//     int x = (screenW - width) / 2;
//     int y = (screenH - height) / 2;

//     SetWindowPos(
//         hWnd, HWND_TOPMOST,
//         x, y, width, height,
//         SWP_SHOWWINDOW
//     );
// }