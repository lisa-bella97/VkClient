#include <iostream>
#include <vk/client.hpp>
#include <vk/json.hpp>

namespace Vk
{
    using json = nlohmann::json;

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
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, buffer);

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

    auto Client::get_friends() -> void
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
                json jsn_items = jsn_response["items"];
                int counter = 0;

                for (json::iterator it = jsn_items.begin(); it != jsn_items.end(); ++it)
                {
                    std::cout << ++counter << ". ";

                    json jsn_id = it.value()["id"];
                    if (!jsn_id.is_null())
                        std::cout << "id" << ": " << jsn_id.begin().value() << std::endl;

                    json jsn_fname = it.value()["first_name"];
                    if (!jsn_fname.is_null())
                        std::cout << "first name" << ": " << jsn_fname.begin().value() << std::endl;

                    json jsn_lname = it.value()["last_name"];
                    if (!jsn_lname.is_null())
                        std::cout << "last name" << ": " << jsn_lname.begin().value() << std::endl;

                    json jsn_bdate = it.value()["bdate"];
                    if (!jsn_bdate.is_null())
                        std::cout << "birthday" << ": " << jsn_bdate.begin().value() << std::endl;

                    json jsn_online = it.value()["online"];
                    if (!jsn_online.is_null())
                        std::cout << "online" << ": " << ((int)jsn_online.begin().value() == 1 ? "yes" : "no") << std::endl;
                }
            }
        }

        curl_easy_reset(_curl);
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