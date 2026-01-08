#ifndef USECASE_CORE_DPMWRAPPER_HPP_
#define USECASE_CORE_DPMWRAPPER_HPP_
#include <filesystem>
#include <fstream>
#include <string>

//-----
//Deep packet modifier wrapper
//-----
namespace usecase {
    class DPMWrapper {
        public:
            DPMWrapper() = default;
            ~DPMWrapper() = default;
            enum STATUS : int {
                OK,
                MISSING_FILE,
                BAD_FILE,
                ERR
            };
            STATUS restruct(const std::filesystem::path&, const std::wstring&); 
        private:
            std::filesystem::path filePath_;
            std::wstring command_;
    };
}













#endif //!USECASE_CORE_DPMWRAPPER_HPP_