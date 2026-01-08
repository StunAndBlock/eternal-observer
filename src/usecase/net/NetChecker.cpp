#include "usecase/net/NetChecker.hpp"




namespace usecase{
    NetChecker::NetChecker() : domains_({L"http://www.google.com", L"http://ya.ru", L"http://ok.ru"}){
    }

    std::pair<
    std::unordered_map< std::wstring, NetChecker::INTERNET_STATUS>,
    NetChecker::STATUS
    > 
    NetChecker::check() {
        DWORD flags;
        BOOL isNotConnected = !InternetGetConnectedState(&flags, 0);
        if (isNotConnected) {
            return {{}, STATUS::NOT_EVEN_CONNECTED};
        }
        HINTERNET hInternet = InternetOpenW(
            L"ObserverInternetNetChecker",
            INTERNET_OPEN_TYPE_PRECONFIG,
            NULL,
            NULL,
            0
        );
        if (!hInternet){
            return {{}, STATUS::ERR};
        }

        flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
        std::unordered_map<std::wstring, NetChecker::INTERNET_STATUS> ichres;
        size_t fails = 0;
        for (const auto& iter : domains_){
            HINTERNET hInternetUrl = InternetOpenUrlW(
                hInternet,
                iter.c_str(),
                NULL,
                0,
                flags,
                0
            );
            if (hInternetUrl != NULL){
                ichres[iter] = INTERNET_STATUS::OK;
                InternetCloseHandle(hInternetUrl);
            } else {
                fails++;
                ichres[iter] = INTERNET_STATUS::FAILED;
            }
        }
        InternetCloseHandle(hInternet);
        if (fails == domains_.size()){
            return {ichres, STATUS::NO_INET_ACCESS}; 
        } 
        return {ichres, STATUS::OK};
    }
}