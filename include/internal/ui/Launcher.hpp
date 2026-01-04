#ifndef UI_LAUNCHER_HPP_
#define UI_LAUNCHER_HPP_
#include "ui/common/WindowBase.hpp"
#include <unordered_map>
namespace ui {
class Launcher : public WindowBase {
    public:
        enum class STAGE_LEVEL {
            STARTING,
            REGISTER_CONFIGURATION,
            PREPARING_CORE,
            NET_WARMUP,
            FINISHING,
            COMPLETED
        };
        Launcher();
        ~Launcher() = default;
        void set_stage(STAGE_LEVEL);
        void testNotification();
    private:
        LRESULT __stdcall handler(UINT, WPARAM, LPARAM) override;
        static constexpr UINT WM_UPDATE_STAGE_ = WM_USER + 100;
        std::unordered_map<STAGE_LEVEL, const wchar_t*> stageRepr_;
        STAGE_LEVEL currentStage_;
};

}
#endif //!UI_LAUNCHER_HPP_