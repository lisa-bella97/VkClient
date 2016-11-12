#include <vk/client.hpp>
#include <iostream>

int main()
{
    std::string token;
    std::cout << "Для получения access_token пройдите по данному url:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.59" << std::endl;
    std::cout << "Access_token = ";
    std::cin >> token;
    Vk::Client vk_cl({{"token", token}});

    if (vk_cl.check_connection())
        std::cout << "Connected." << std::endl;

    vk_cl.get_friends();

    return 0;
}