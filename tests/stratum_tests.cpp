#include "stratum.h"

#include <gtest/gtest.h>
#include <thread>

// TEST(StratumTests, BasicConnect)
// {
//     // auto client = Util::StratumClient::NewClient("eth-us-east.flexpool.io", 4444);
//     EXPECT_TRUE(Util::StratumClient::NewClient("www.google.com", 80)->Connect());
// }

// TEST(StratumTests, TestFetchGoogle)
// {
//     // auto client = Util::StratumClient::NewClient("eth-us-east.flexpool.io", 4444);
//     auto client = stratum::TcpSocket::Open("www.google.com", 80);
//     EXPECT_TRUE(client->Connect());
//     const auto getBody = "GET / HTTP/1.1\r\n\r\n";
//     EXPECT_TRUE(client->Send((void *)getBody, strlen(getBody)));

//     auto response = client->Receive();
//     EXPECT_TRUE(response.has_value());
//     if (response.has_value())
//     {
//         std::cout << &response->get()->buffer[0] << std::endl;
//     }
// }


TEST(StratumTests, PingFlexpool)
{
    bool handshakeSuccessful = false;
    bool authSuccessful = false;
    //auto client = stratum::StratumClient::NewClient("eth-us-east.flexpool.io", 4444);
    auto config = stratum::stratum_config{};
    config.host = "na.luckpool.net";
    config.port = 3357;
    config.user = "test_user";
    config.pass = "x";
    auto client = stratum::stratum_client::NewClient(config);
    client->setCallback(stratum::CallbackTypeE::CALLBACKTYPE_HANDSHAKE, [&](const auto& client){
        handshakeSuccessful = true;
    });
    client->setCallback(stratum::CallbackTypeE::CALLBACKTYPE_AUTH, [&](const auto& client){
        authSuccessful = true;
    });
    client->begin();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    EXPECT_TRUE(handshakeSuccessful);
    EXPECT_TRUE(authSuccessful);
}

TEST(HelloTest, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
