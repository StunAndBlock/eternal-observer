#include "usecase/config/Config.hpp"


usecase::Config::Config() : varsRepr_({L"dpm_path",L"dpm_args"}){
}


const struct usecase::config& usecase::Config::getConfig() const{
    return configData_;
}

struct usecase::config& usecase::Config::getModifiableConfig(){
    return configData_;
}
const std::vector<std::wstring>& usecase::Config::getRepresentation() const {
    return varsRepr_;
}