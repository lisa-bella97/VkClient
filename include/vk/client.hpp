#ifndef VKCLIENT_CLIENT_HPP
#define VKCLIENT_CLIENT_HPP

#include <curl/curl.h>
#include "json.hpp"
#include <string>
#include <map>

#define USE_AUTH_CODE_FLOW // Используется Authorization Code Flow для получения ключа доступа пользователя; иначе - Implicit Flow

namespace Vk
{
    class Client
    {
    public:
        using dict_t = std::map<std::string, std::string>;
        using json = nlohmann::json;

        Client() : _curl(curl_easy_init()) {}
        Client(const dict_t & settings);
        ~Client() { curl_easy_cleanup(_curl); }
        auto check_connection() -> bool;
        auto get_friends() -> json;
        auto set_settings(const dict_t & settings) -> void { _settings = settings; }

    private:
        CURL * _curl;
        dict_t _settings;

        static auto write_callback(char * data, size_t size, size_t nmemb, void * buff) -> size_t;
    };
}

#endif