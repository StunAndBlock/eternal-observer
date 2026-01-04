#include "usecase/config/Config.hpp"


usecase::Config::Config() : varsRepr_({L"cmd"}){
}


struct usecase::config& usecase::Config::getModifiableConfig(){
    return configData_;
}
const std::vector<std::wstring>& usecase::Config::getRepresentation() const {
    return varsRepr_;
}