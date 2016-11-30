#include <vk/client.hpp>

int main(int argc, char ** argv)
{
#ifdef USE_AUTH_CODE_FLOW
    Vk::Client vk_cl({{"code", std::getenv("CODE")}});
#else
    Vk::Client vk_cl({{"token", std::getenv("TOKEN")}});
#endif

    if (vk_cl.check_connection())
        std::cout << "Connected." << std::endl;

    Vk::Client::sync_print_friends(vk_cl.get_friends(), atoi(std::getenv("NUMBER")), argc == 2 && strcmp(argv[1], "-v") == 0);

    return 0;
}