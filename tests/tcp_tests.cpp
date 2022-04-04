#include "stratum.h"

#include "socket.h"
#include <gtest/gtest.h>
#include <thread>


TEST(StratumTests, TestFetchGoogle)
{
    auto socket = stratum::tcp_socket::Open("www.google.com", 80);
    EXPECT_TRUE(socket->connect());
    const auto getBody = "GET / HTTP/1.1\r\n\r\n";
    EXPECT_TRUE(socket->send((void *)getBody, strlen(getBody)));

    auto response = socket->Receive();
    EXPECT_TRUE(response.has_value());
    if (response.has_value())
    {
        std::cout << &response->get()->buffer[0] << std::endl;
    }
}
