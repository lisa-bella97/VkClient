#include <vk/client.hpp>
#include <iostream>
#include "catch.hpp"

#ifdef USE_AUTH_CODE_FLOW
SCENARIO("client must check connection using an authorization code")
{
    GIVEN("an invalid authorization code")
    {
        std::string invalid_code = "123";

        WHEN("initialize client")
        {
            Vk::Client client(invalid_code);

            THEN("check_connection() must return false")
            {
                REQUIRE(!client.check_connection());
            }
        }
    }

    GIVEN("a valid authorization code")
    {
        std::string valid_code = getenv("VALID_CODE");

        WHEN("initialize client")
        {
            Vk::Client client(valid_code);

            THEN("check_connection() must return true")
            {
                REQUIRE(client.check_connection());
            }
        }
    }
}
#else
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
        std::map<std::string, std::string> valid_settings;
        valid_settings["token"] = getenv("VALID_TOKEN");

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
#endif