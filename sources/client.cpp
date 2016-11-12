#include <curl/curl.h>
#include <iostream>
#include <vk/client.hpp>
#include <vk/json.hpp>

namespace Vk
{
    using json = nlohmann::json;

    auto Client::check_connection() -> bool
    {
        CURL *curl = curl_easy_init();

        if (curl)
        {
            std::string fields = "access_token=" + _settings["token"] + "&v=5.59";
            std::cout << _settings["token"] << std::endl;
            std::string buffer = "";

            curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
            std::cout << "curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);" << std::endl;

            if (curl_easy_perform(curl) == CURLE_OK)
            {
                std::cout << "if (curl_easy_perform(curl) == CURLE_OK)" << std::endl;
                std::cout << buffer << std::endl;
                json jsn_obj = json::parse(buffer);
                json jsn_response = jsn_obj["response"];

                if (!jsn_response.is_null())
                {
                    std::cout << "if (!jsn_response.is_null())" << std::endl;
                    curl_easy_cleanup(curl);
                    return true;
                }
            }
        }

        curl_easy_cleanup(curl);
        std::cout << "curl_easy_cleanup(curl); return false" << std::endl;
        return false;
    }

    auto Client::get_friends() -> void
    {
        CURL *curl = curl_easy_init();

        if (curl)
        {
            std::string fields = "fields=bdate&access_token=" + _settings["token"] + "&v=5.59";
            std::string buffer = "";

            curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/friends.get?");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

            if (curl_easy_perform(curl) == CURLE_OK)
            {
                json jsn_obj = json::parse(buffer);
                json jsn_response = jsn_obj["response"];
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
                        std::cout << "online" << ": " << (jsn_online.begin().value() == 1 ? "yes" : "no") << std::endl;
                }
            }
        }

        curl_easy_cleanup(curl);
    }

    auto Client::write_callback(char * data, size_t size, size_t nmemb, std::string & buff) -> size_t
    {
        size_t result = 0;

        if (buff.c_str())
        {
            buff += data;
            result = size * nmemb;
        }

        return result;
    }
}
