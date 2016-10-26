#include <vk/client.hpp>
#include <iostream>
#include <curl/curl.h>
#include <boost/regex.hpp>

namespace Vk
{
	auto Client::write_callback(char * data, size_t size, size_t nmemb, std::string& buff) -> int
	{
		int result = 0;

		if (buff.c_str())
		{
			buff.append(data, size * nmemb);
			result = size * nmemb;
		}

		return result;
	}

	auto Client::check_connection() -> bool
	{
		CURL *curl = curl_easy_init();

		if (curl)
		{
			std::string fields = "access_token=" + _token + "&v=5.59";
			std::string buffer = "";

			curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

			if (curl_easy_perform(curl) == CURLE_OK)
			{
				boost::regex pattern("^{\"response\"(.*)}$");
				boost::smatch matches;

				if (boost::regex_match(buffer, matches, pattern))
				{
					curl_easy_cleanup(curl);
					return true;
				}
			}
		}

		curl_easy_cleanup(curl);
		return false;
	}
}
