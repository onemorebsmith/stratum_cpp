#include "stratum_request.h"

#include "stratum.h"

// atomic counter for generating request ids
static std::atomic_int requestCounter;

int newRequestId()
{
    return requestCounter.fetch_add(1);
}

namespace stratum
{
stratum_request::stratum_request(stratum_client &c, const std::string &_method)
    : client{c}, method{_method}, handler{}, id{newRequestId()}
{
}

std::string stratum_request::toString() const
{
    return nlohmann::json{{"id", id}, {"method", method}, {"params", paramArray}}.dump() + "\n";
}

void stratum_request::dispatch(const stratum_response &response) const
{
    handler(response);
}

} // namespace stratum