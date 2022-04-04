#include "stratum.h"

#include "logger.h"
#include "test_miner.h"

namespace stratum_test
{
auto logger = stratum::logger("miner");

struct miningState
{
    std::string job;
    std::string version;
    std::vector<unsigned char> header;
};
miningState state = {};

std::pair<int32_t, bool> parse_version(std::string_view in)
{
    try
    {
        const auto version = be32toh(std::stoi(in.data(), 0, 16));
        return std::make_pair(version, true);
    }
    catch (std::exception)
    {
        return std::make_pair(0, false);
    }
}

constexpr char charToHex(char c) {
    c -= 48;

    return static_cast<unsigned char>(c);
}

void test_miner::start(const stratum::stratum_config &config)
{
    state.header.reserve(1024);

    // adapted from nheqminer
    client = stratum::stratum_client::newClient(config);
    client->onMiningNotify([&](const stratum::stratum_client &client, const stratum::mining_notify &notification) {
        logger.debug() << "mining notify";
        state.job = notification.params[0];
        const auto [version, ok] = parse_version(notification.params[1]);
        if (!ok)
        {
            logger.err() << "failed parsing version from notifiy header";
            return;
        }
        if (version == 4 || version == 0x20000000 || version == 65540)
        {
            const auto params = notification.params;
            const auto header = params[1] + params[2] + params[3] + params[4] + params[5] + params[6] +
                                // no nonce + no solution
                                "0000000000000000000000000000000000000000000000000000000000000000" + "00";
            state.header.clear();
            for (auto c : header)
            {
                if (isspace(c)) continue;

            }
        }
    });
    client->begin();
    client->join();
}

} // namespace stratum_test