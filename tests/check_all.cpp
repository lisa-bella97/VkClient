#include <vk/client.hpp>
#include "catch.hpp"

Vk::Client client;

SCENARIO("client must check connection using settings")
{
    Vk::Client::dict_t settings;

    GIVEN("invalid settings")
    {
#ifdef USE_AUTH_CODE_FLOW
        settings["code"] = "123";
#else
        settings["token"] = "123";
#endif

        WHEN("initialize client")
        {
            client.set_settings(settings);

            THEN("check_connection() must return false")
            {
                REQUIRE(!client.check_connection());
            }
        }
    }

    GIVEN("valid settings")
    {
#ifdef USE_AUTH_CODE_FLOW
        settings["code"] = getenv("VALID_CODE");
#else
        settings["token"] = getenv("VALID_TOKEN");
#endif

        WHEN("initialize client")
        {
            client.set_settings(settings);

            THEN("check_connection() must return true")
            {
                REQUIRE(client.check_connection());
            }
        }
    }
}

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