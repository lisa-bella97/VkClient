#include <vk/client.hpp>
#include <iostream>

int main()
{
	std::string token;
	std::cout << "Access_token = "; // скопировать после выполнения запроса https://oauth.vk.com/authorize?client_id=5682862&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.59
	std::cin >> token;

	Vk::Client vk_cl(token);

	if (vk_cl.check_connection())
        	std::cout << "Connected." << std::endl;

	return 0;
}