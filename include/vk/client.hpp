#ifndef VKCLIENT_CLIENT_HPP
#define VKCLIENT_CLIENT_HPP

#include <string>

namespace Vk
{
	class Client
	{
	public:
   		Client(std::string token) : _token(token) {}
   		auto check_connection() -> bool;

	private:
        	std::string _token;
        	static auto write_callback(char * data, size_t size, size_t nmemb, std::string& buff) -> int;
	};
}

#endif
