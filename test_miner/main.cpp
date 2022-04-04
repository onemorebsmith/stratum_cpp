
#include "test_miner.h"

int main()
{
   const auto conf =  stratum::stratum_config{
        .host = "na.luckpool.net", .port = 3357, .userAgent = "test_ua", .user = "test_user", .pass = "x"};

    stratum_test::test_miner miner =  {};
    miner.start(conf);
}