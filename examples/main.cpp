#include <vk/client.hpp>

void print_friends(const Vk::Client::json & friends);

int main()
{
#ifdef USE_AUTH_CODE_FLOW
    std::string code;
    std::cout << "Для получения code пройдите по данному url:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=code&v=5.60" << std::endl;
    std::cout << "code = ";
    std::cin >> code;
    Vk::Client vk_cl({{"code", code}});
#else
    std::string token;
    std::cout << "Для получения access_token пройдите по данному url:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.60" << std::endl;
    std::cout << "Access_token = ";
    std::cin >> token;
    Vk::Client vk_cl({{"token", token}});
#endif

    if (vk_cl.check_connection())
        std::cout << "Connected." << std::endl;

    print_friends(vk_cl.get_friends());

    return 0;
}

void print_friends(const Vk::Client::json & friends)
{
    if (!friends.is_null())
    {
        int counter = 0;

        for (Vk::Client::json::const_iterator it = friends.begin(); it != friends.end(); ++it)
        {
            std::cout << ++counter << ". ";

            Vk::Client::json jsn_id = it.value()["id"];
            if (!jsn_id.is_null())
                std::cout << "id" << ": " << jsn_id.begin().value() << std::endl;

            Vk::Client::json jsn_fname = it.value()["first_name"];
            if (!jsn_fname.is_null())
                std::cout << "first name" << ": " << jsn_fname.begin().value() << std::endl;

            Vk::Client::json jsn_lname = it.value()["last_name"];
            if (!jsn_lname.is_null())
                std::cout << "last name" << ": " << jsn_lname.begin().value() << std::endl;

            Vk::Client::json jsn_bdate = it.value()["bdate"];
            if (!jsn_bdate.is_null())
                std::cout << "birthday" << ": " << jsn_bdate.begin().value() << std::endl;

            Vk::Client::json jsn_online = it.value()["online"];
            if (!jsn_online.is_null())
                std::cout << "online" << ": " << ((int)jsn_online.begin().value() == 1 ? "yes" : "no") << std::endl;
        }
    }
}