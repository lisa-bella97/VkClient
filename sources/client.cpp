#include <iostream>
#include <vk/client.hpp>

namespace Vk
{
    auto Client::check_connection() -> bool
    {
        if (_curl)
        {
#ifdef USE_AUTH_CODE_FLOW
            std::string fields = "client_id=5682862&client_secret=" + std::string(getenv("CL_SECRET")) +"&redirect_uri=https://oauth.vk.com/blank.html&code=" + _code;
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://oauth.vk.com/access_token?");
            curl_easy_setopt(_curl, CURLOPT_POST, 1L);
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                json jsn_token = (json::parse(buffer))["access_token"];

                if (!jsn_token.is_null())
                {
                    _settings["token"] = jsn_token.begin().value();
                    curl_easy_reset(_curl);
                    return true;
                }
            }
#else
            std::string fields = "access_token=" + _settings["token"] + "&v=5.60";
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
            curl_easy_setopt(_curl, CURLOPT_POST, 1L);
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                json jsn_response = (json::parse(buffer))["response"];

                if (!jsn_response.is_null())
                {
                    curl_easy_reset(_curl);
                    return true;
                }
            }
#endif
        }

        curl_easy_reset(_curl);
        return false;
    }

    auto Client::get_friends() -> Client::json
    {
        if (_curl)
        {
            std::string fields = "fields=bdate&access_token=" + _settings["token"] + "&v=5.60";
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://api.vk.com/method/friends.get?");
            curl_easy_setopt(_curl, CURLOPT_POST, 1L);
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                json jsn_response = (json::parse(buffer))["response"];
                curl_easy_reset(_curl);
                return jsn_response["items"];
            }
        }

        curl_easy_reset(_curl);
        return nullptr;
    }

    auto Client::write_callback(char * data, size_t size, size_t nmemb, void * buff) -> size_t
    {
        size_t result = 0;

        if (buff)
        {
            result = size * nmemb;
            ((std::string*)buff)->append(data, 0, result);
        }

        return result;
    }
}