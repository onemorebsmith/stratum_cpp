#include "stratum.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <thread>

#include "logger.h"
#include "stratum_protocol.hpp"
#include "stratum_request.h"
#include "stratum_response.h"

namespace stratum
{

static auto clientLogger = stratum::logger("[stratum]");
static constexpr auto userAgent = "chaos_miner";

bool resolve_dns(std::string_view host, addrinfo &out);

std::unique_ptr<stratum_client> stratum_client::newClient(const stratum_config &params)
{
    return std::unique_ptr<stratum_client>(new stratum_client(params));
}

stratum_client::stratum_client(const stratum_config &params)
{
    protocol = std::make_unique<stratum_protocol>(params, *this);
}

stratum_client::~stratum_client()
{
    protocol->stop();
}

bool stratum_client::begin()
{
    protocol->begin();
    return false;
}

bool stratum_client::execRequest(RequestHandle req)
{
    return protocol->execRequest(req);
}

void stratum_client::onMiningNotify(mining_notify_cb_t &&cb)
{
    protocol->onMiningNotify(std::forward<mining_notify_cb_t>(cb));
}

void stratum_client::setCallback(CallbackTypeE type, callback_t &&cb)
{
    protocol->setCallback(type, std::forward<callback_t>(cb));
};

void stratum_client::dispatchCallback(CallbackTypeE type) const
{
    protocol->dispatchCallback(type);
}

void stratum_client::join()
{
    protocol->join();
}

} // namespace stratum
