#ifndef USECASE_CONFIG_CONFIG_HPP_
#define USECASE_CONFIG_CONFIG_HPP_

#include <string>
#include <vector>

namespace usecase {


    struct config {
        std::wstring observerCoreCmd_;
    };

    class Config {
        public:
            Config();
            const struct config& getConfig() const;
            struct config& getModifiableConfig();
            const std::vector<std::wstring>& getRepresentation() const;
        private:
            struct config configData_;
            const std::vector<std::wstring> varsRepr_;
    };
    
} 
#endif 

//!USECASE_CONFIG_CONFIG_HPP_