#include <vk/client.hpp>
#include <iostream>

int main()
{
#ifdef USE_AUTH_CODE_FLOW
    std::string code;
    std::cout << "Для получения code пройдите по данному url:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=code&v=5.60" << std::endl;
    std::cout << "code = ";
    std::cin >> code;
    Vk::Client vk_cl(code);
#else
    std::string token;
    //std::cout << "Для получения access_token пройдите по данному url:" << std::endl;
    //std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.60" << std::endl;
    //std::cout << "Access_token = ";
    token = "59fb3a7570356fa0837f7db32ed04b2fafa8c7ea8d347d2c95ca35352d87a91b5343f29cb415948168489";
    //std::cin >> token;
    Vk::Client vk_cl({{"token", token}});
#endif

    if (vk_cl.check_connection())
        std::cout << "Connected." << std::endl;

    vk_cl.get_friends();

    return 0;
}