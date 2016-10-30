#include <vk/client.hpp>
#include "validInit.hpp"
#include "catch.hpp"

SCENARIO("client must check connection using a token")
{
    GIVEN("an invalid token")
    {
        std::map<std::string, std::string> invalid_settings{{"token", "123"}};

        WHEN("initialize client")
        {
            Vk::Client client(invalid_settings);

            THEN("check_connection() must return false")
            {
                REQUIRE(!client.check_connection());
            }
        }
    }

    GIVEN("a valid token")
    {
        std::map<std::string, std::string> valid_settings(get_valid_settings());

        WHEN("initialize client")
        {
            Vk::Client client(valid_settings);

            THEN("check_connection() must return true")
            {
                REQUIRE(client.check_connection());
            }
        }
    }
}