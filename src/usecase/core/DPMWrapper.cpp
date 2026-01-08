#include "usecase/core/DPMWrapper.hpp"

namespace usecase {

    DPMWrapper::STATUS DPMWrapper::restruct(const std::filesystem::path& filePath, const std::wstring& commandArguments){
        if (!std::filesystem::exists(filePath)){
            return STATUS::MISSING_FILE;
        }
        
        filePath_ = filePath;
        command_ = filePath.wstring() + commandArguments;
        return STATUS::OK;
    }
}