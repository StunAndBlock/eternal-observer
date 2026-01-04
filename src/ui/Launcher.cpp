#include "ui/Launcher.hpp"

ui::Launcher::Launcher(){
    stageRepr_ = {
        {STAGE_LEVEL::STARTING, L"starting loadup"},
        {STAGE_LEVEL::REGISTER_CONFIGURATION, L"stage: configurating"},
        {STAGE_LEVEL::PREPARING_CORE, L"stage: preparing core"},
        {STAGE_LEVEL::NET_WARMUP, L"stage: warmaping network" },
        {STAGE_LEVEL::FINISHING, L"stage: polishing and syncing"},
        {STAGE_LEVEL::COMPLETED, L"loadup finished"}
    };
    currentStage_ = STAGE_LEVEL::STARTING;
}

void ui::Launcher::set_stage(const STAGE_LEVEL sl){
    PostMessage(wnd_, WM_UPDATE_STAGE_, NULL, (LPARAM)sl);
}


void ui::Launcher::testNotification(){
    MessageBox(wnd_, L"detected first start, be sure to configure application later", L"", 
                MB_YESNO | MB_ICONQUESTION);
}


LRESULT __stdcall ui::Launcher::handler(UINT msg , WPARAM wParam, LPARAM lParam){
    switch (msg)
    {
    case WM_CREATE:
       // SetWindowOpacity(wnd_, 200);
        ShowWindow(wnd_, SW_SHOW);
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(wnd_, &ps);

        RECT rc;
        GetClientRect(wnd_, &rc);

        // Background
        HBRUSH bg = CreateSolidBrush(RGB(15, 15, 20));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);
        SetTextColor(hdc, RGB(0, 0, 0));     // Red text
        SetBkColor(hdc, RGB(255, 255, 0));     // Yellow background
        SetBkMode(hdc, OPAQUE);               // Opaque background
        RECT rect = {50, 100, 400, 200};
        DrawText(hdc, L"LDLP Observer construction", 
                    -1, &rect, DT_SINGLELINE | DT_VCENTER);
        // HFONT hFont = CreateFont(
        //     16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        //     RUSSIAN_CHARSET,  // or DEFAULT_CHARSET
        //     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        //     DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        //     L"Arial"  // Use font that supports Cyrillic
        // );
        // HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        // DrawTextW(hdc, L"ИЛЬЯ ГЕЙ", 
        //             -1, &rect, DT_SINGLELINE | DT_VCENTER);
        // SelectObject(hdc, hOldFont);
        rect = {50, 150, 400, 200};
        DrawText(hdc, stageRepr_[currentStage_], 
                    -1, &rect, DT_SINGLELINE | DT_VCENTER);
        EndPaint(wnd_, &ps);
    }
    break;
    case WM_UPDATE_STAGE_: {
        currentStage_ = (STAGE_LEVEL)lParam;
        InvalidateRect(wnd_, NULL, TRUE);
        UpdateWindow(wnd_);
        break;
    }
    case WM_CLOSE: 
        if (MessageBox(wnd_, L"Force stoping launcher can lead to app data corruption, are you sure?", L"", 
                       MB_YESNO | MB_ICONQUESTION) == IDYES){
            DestroyWindow(wnd_);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(wnd_, msg, wParam, lParam);
    }
    return DefWindowProc(wnd_, msg, wParam, lParam);
}