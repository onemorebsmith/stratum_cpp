#include "stratum.h"

#include <gtest/gtest.h>
#include <thread>

bool testPool(const stratum::stratum_config &config)
{
    bool handshakeSuccessful = false;
    bool authSuccessful = false;
    bool notifyReceived = false;
    // auto client = stratum::StratumClient::NewClient("eth-us-east.flexpool.io", 4444);
    auto client = stratum::stratum_client::newClient(config);
    client->setCallback(stratum::CallbackTypeE::CALLBACKTYPE_HANDSHAKE,
                        [&](const auto &client) { handshakeSuccessful = true; });
    client->setCallback(stratum::CallbackTypeE::CALLBACKTYPE_AUTH, [&](const auto &client) { authSuccessful = true; });
    client->onMiningNotify([&](const auto &client, const stratum::mining_notify &notif) { notifyReceived = true; });
    client->begin();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    EXPECT_TRUE(handshakeSuccessful);
    EXPECT_TRUE(authSuccessful);
    // EXPECT_TRUE(notifyReceived); 
    return true;
}

TEST(StratumTests, ping_real_pool)
{
    const stratum::stratum_config pools[] = {
        stratum::stratum_config{
            .host = "na.luckpool.net", .port = 3357, .userAgent = "test_ua", .user = "test_user", .pass = "x"},
        // stratum::stratum_config{  // TODO (bs): ethermine is doesn't like our handshake for some reason
        //     .host = "us1.ethermine.org", .port = 4444, .userAgent = "test_ua", .user = "test_user", .pass = "x"},
        // stratum::stratum_config{  // TODO (bs): hivepool doesn't respond... at all
        //    .host = "eth.hiveon.com", .port = 4444, .userAgent = "test_ua", .user = "test_user", .pass = "x"},
        stratum::stratum_config{
            .host = "etc-us-east.flexpool.io", .port = 4444, .userAgent = "test_ua", .user = "test_user", .pass = "x"}};

    std::thread threads[std::size(pools)] = {};
    int idx = 0;
    for (const auto &cfg : pools)
    {
        threads[idx++] = std::thread([&cfg] { EXPECT_TRUE(testPool(cfg)); });
    }

    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}
