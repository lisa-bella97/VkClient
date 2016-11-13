#include <vk/client.hpp>
#include "catch.hpp"

Vk::Client client;

#ifdef USE_AUTH_CODE_FLOW
SCENARIO("client must check connection using an authorization code")
{
    GIVEN("an invalid authorization code")
    {
        std::string invalid_code = "123";

        WHEN("initialize client")
        {
            client.set_settings(invalid_code);

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
            client.set_settings(valid_code);

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
            client.set_settings(invalid_settings);

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
            client.set_settings(valid_settings);

            THEN("check_connection() must return true")
            {
                REQUIRE(client.check_connection());
            }
        }
    }
}
#endif

SCENARIO("client must get friends correctly")
{
    GIVEN("an authorized client and a json object with his friends")
    {
        Vk::Client::json friends = R"([{"bdate":"5.4.1997","first_name":"Елизавета","id":126643967,"last_name":"Прохорова","online":0}])"_json;

        WHEN("get friends")
        {
            Vk::Client::json friends_from_get = client.get_friends();
            bool are_same = (friends == friends_from_get);

            THEN("json objects must be the same")
            {
                REQUIRE(are_same);
            }
        }
    }
}