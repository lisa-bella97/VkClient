#include <vk/client.hpp>

int main(int argc, char ** argv)
{
#ifdef USE_AUTH_CODE_FLOW
    std::string code;
    std::cout << "Для получения code перейдите по данному URL:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=code&v=5.60" << std::endl;
    std::cout << "code = ";
    std::cin >> code;

    if (!std::cin)
        std::cerr << "Wrong input" << std::endl;

    Vk::Client vk_cl({{"code", code}});
#else
    std::string token;
    std::cout << "Для получения access_token перейдите по данному URL:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.60" << std::endl;
    std::cout << "Access_token = ";
    std::cin >> token;

    if (!std::cin)
        std::cerr << "Wrong input" << std::endl;

    Vk::Client vk_cl({{"token", token}});
#endif

    if (vk_cl.check_connection())
        std::cout << "Connected." << std::endl;

    size_t number;
    std::cout <<  "Enter the number of threads:" << std::endl;
    std::cin >> number;

    Vk::Client::sync_print_friends(vk_cl.get_friends(), number, argc == 2 && strcmp(argv[1], "-v") == 0);

    return 0;
}