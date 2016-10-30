#ifndef VKCLIENT_VALIDINIT_HPP
#define VKCLIENT_VALIDINIT_HPP

#include <string>
#include <map>
#include <iostream>

std::map<std::string, std::string> get_valid_settings()
{
    std::string token;
    std::cout << "Для получения access_token пройдите по данному url:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.59" << std::endl;
    std::cout << "Access_token = ";
    std::cin >> token;

    return std::map<std::string, std::string>({{"token", token}});
};

#endif