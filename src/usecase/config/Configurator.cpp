#include "usecase/config/Configurator.hpp"


usecase::Configurator::Configurator(const wchar_t* fileName){
    fileName_ = fileName;
}

usecase::Configurator::~Configurator(){
    if (fileDiscriptor_.is_open()){
        fileDiscriptor_.close();
    }
}
usecase::Configurator::STATUS usecase::Configurator::load(struct config& data, const std::vector<std::wstring>& repr){
    STATUS code = STATUS::OK;
    if (!std::filesystem::exists(fileName_)){
        code = STATUS::FIRST_START;
    }

    fileDiscriptor_.open(fileName_, std::ios::in | std::ios::out | std::ios::app);
    if (!fileDiscriptor_.good()){
        return STATUS::ERR;
    }
    if (code != STATUS::FIRST_START) {
        CFG::ReadFile(fileDiscriptor_, repr, data.observerCoreCmd_);
    }
    return code;
}


usecase::Configurator::STATUS usecase::Configurator::save(const struct config& data, const std::vector<std::wstring>& repr){
    if (!fileDiscriptor_.good()){
        return STATUS::ERR;
    }
    CFG::WriteFile(fileDiscriptor_, repr, data.observerCoreCmd_);
    return STATUS::OK;
}