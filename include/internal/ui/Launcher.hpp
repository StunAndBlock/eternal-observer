#ifndef UI_LAUNCHER_HPP_
#define UI_LAUNCHER_HPP_
#include "ui/common/WindowBase.hpp"
#include "usecase/launcher/Interface.hpp"
#include <unordered_map>
namespace ui {
class Launcher : public WindowBase {
    public:
        Launcher();
        ~Launcher() = default;
        void set_stage(usecase::ILauncher::STAGE_LEVEL);
        void testNotification();
        void fatalErrorNotification();
    private:
        LRESULT __stdcall handler(UINT, WPARAM, LPARAM) override;
        static constexpr UINT WM_UPDATE_STAGE_ = WM_USER + 100;
        std::unordered_map<usecase::ILauncher::STAGE_LEVEL, const wchar_t*> stageRepr_;
        usecase::ILauncher::STAGE_LEVEL currentStage_;
};

}
#endif //!UI_LAUNCHER_HPP_