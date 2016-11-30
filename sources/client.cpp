#include <iostream>
#include <thread>
#include <vector>
#include <vk/client.hpp>

namespace Vk
{
    std::mutex mutex;

    Client::Client(const dict_t & settings) : _curl(curl_easy_init()), _settings(settings)
    {
        curl_easy_setopt(_curl, CURLOPT_POST, 1L);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);

#ifdef USE_AUTH_CODE_FLOW
        if (_curl)
        {
            std::string fields = "client_id=5682862&client_secret=" + std::string(getenv("CL_SECRET")) +
                                 "&redirect_uri=https://oauth.vk.com/blank.html&code=" + _settings["code"];
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://oauth.vk.com/access_token?");
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                try
                {
                    json jsn_token = (json::parse(buffer))["access_token"];

                    if (!jsn_token.is_null())
                        _settings["token"] = jsn_token.begin().value();
                }
                catch (const std::exception & ex)
                {
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
#endif
    }

    auto Client::check_connection() -> bool
    {
        if (_curl)
        {
            std::string fields = "access_token=" + _settings["token"] + "&v=5.60";
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                try
                {
                    json jsn_response = (json::parse(buffer))["response"];

                    if (!jsn_response.is_null())
                        return true;
                }
                catch (const std::exception & ex)
                {
                    std::cerr << ex.what() << std::endl;
                }
            }
        }

        return false;
    }

    auto Client::get_friends() -> Client::json
    {
        if (_curl)
        {
            std::string fields = "fields=bdate&access_token=" + _settings["token"] + "&v=5.60";
            std::string buffer = "";

            curl_easy_setopt(_curl, CURLOPT_URL, "https://api.vk.com/method/friends.get?");
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

            if (curl_easy_perform(_curl) == CURLE_OK)
            {
                try
                {
                    json jsn_response = (json::parse(buffer))["response"];
                    return jsn_response["items"];
                }
                catch (const std::exception &ex)
                {
                    std::cerr << ex.what() << std::endl;
                }
            }
        }

        return nullptr;
    }

    auto Client::sync_print_friends(const Vk::Client::json & friends, size_t threads_count, bool is_debug) -> void
    {
        if (threads_count < 1 || threads_count > std::thread::hardware_concurrency())
        {
            std::cout << "Wrong number of threads." << std::endl;
            return;
        }

        if (!friends.is_null())
        {
            auto print_func = [&friends, is_debug, threads_count](int current_index)
            {
                auto friends_size = friends.size();
                auto thread_index = 0;

                for (auto i = current_index; i < friends_size; i += threads_count)
                {
                    while (thread_index != current_index)
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));

                    std::lock_guard<std::mutex> lock(mutex);

                    if (is_debug)
                    {
                        std::time_t timer = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        std::cout << "Thread " << current_index + 1 << std::endl << "Start time: " << ctime(&timer);
                    }

                    std::cout << i + 1 << ". ";
                    auto value = friends.at(i);

                    Vk::Client::json jsn_id = value["id"];
                    if (!jsn_id.is_null())
                        std::cout << "id: " << jsn_id.begin().value() << std::endl;

                    Vk::Client::json jsn_fname = value["first_name"];
                    if (!jsn_fname.is_null())
                        std::cout << "first name: " << jsn_fname.begin().value() << std::endl;

                    Vk::Client::json jsn_lname = value["last_name"];
                    if (!jsn_lname.is_null())
                        std::cout << "last name: " << jsn_lname.begin().value() << std::endl;

                    if (value.find("bdate") != value.end())
                    {
                        Vk::Client::json jsn_bdate = value["bdate"];
                        if (!jsn_bdate.is_null())
                            std::cout << "birthday: " << jsn_bdate.begin().value() << std::endl;
                    }

                    Vk::Client::json jsn_online = value["online"];
                    if (!jsn_online.is_null())
                        std::cout << "online: " << ((int)jsn_online.begin().value() == 1 ? "yes" : "no") << std::endl;

                    if (thread_index < threads_count - 1)
                        thread_index++;
                    else
                        thread_index = 0;

                    if (is_debug)
                    {
                        std::time_t timer = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        std::cout << "End time: " << ctime(&timer) << std::endl;
                    }
                }
            };

            std::vector<std::thread> threads;

            for (auto i = 0; i < threads_count; i++)
                threads.push_back(std::thread(print_func, i));

            for (auto i = 0; i < threads_count; i++)
                if (threads[i].joinable())
                    threads[i].join();
        }
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
