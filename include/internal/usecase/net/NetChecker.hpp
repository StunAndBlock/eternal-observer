#ifndef USECASE_NET_NETCHECKER_HPP_
#define USECASE_NET_NETCHECKER_HPP_
#include <windows.h>
#include <netlistmgr.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <unordered_map>
namespace usecase {
    class NetChecker {
        public:
            enum STATUS : int {
                OK,
                NOT_EVEN_CONNECTED,
                NO_INET_ACCESS,
                ERR
            };
            enum class INTERNET_STATUS {
                OK,
                NOT_CHECKED,
                FAILED
            };
            NetChecker();
            ~NetChecker() = default;
            std::pair<
            std::unordered_map<std::wstring, NetChecker::INTERNET_STATUS>,
            NetChecker::STATUS
            > check();
        private:
            const std::vector<std::wstring> domains_;
        
        };
}











#endif //!USECASE_NET_NETCHECKER_HPP_