#ifndef USECASE_CONFIG_CONFIGURATOR_HPP_
#define USECASE_CONFIG_CONFIGURATOR_HPP_
#include <fstream>
#include "config_file.h"
#include <filesystem>
#include "usecase/config/Config.hpp"
namespace usecase {
    class Configurator {
        public:
            enum STATUS : int {
                OK,
                ERR,
                FIRST_START
            };
            Configurator(const wchar_t*);
            ~Configurator();
            STATUS load(struct config&, const std::vector<std::wstring>&);
            STATUS save(const struct config&, const std::vector<std::wstring>&);
        private:
            std::wfstream fileDiscriptor_;
            const wchar_t* fileName_;
        };

}













#endif //!USECASE_CONFIG_CONFIGURATOR_HPP_
