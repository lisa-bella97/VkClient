#ifndef VKCLIENT_CLIENT_HPP
#define VKCLIENT_CLIENT_HPP

#include <curl/curl.h>
#include <string>
#include <map>

#define USE_AUTH_CODE_FLOW // Используется Authorization Code Flow для получения ключа доступа пользователя; иначе - Implicit Flow

namespace Vk
{
    class Client
    {
    public:
        using dict_t = std::map<std::string, std::string>;

#ifdef USE_AUTH_CODE_FLOW
        Client(std::string code) : _code(code), _curl(curl_easy_init()) {}
#else
        Client(dict_t settings) : _settings(settings), _curl(curl_easy_init()) {}
#endif

        ~Client() { curl_easy_cleanup(_curl); }
        auto check_connection() -> bool;
        auto get_friends() -> void;

    private:
        CURL * _curl;
        dict_t _settings;

#ifdef USE_AUTH_CODE_FLOW
        std::string _code;
#endif

        static auto write_callback(char * data, size_t size, size_t nmemb, std::string & buff) -> size_t;
    };
}

#endif