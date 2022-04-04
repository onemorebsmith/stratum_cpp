#pragma once

#include "types.h"

namespace stratum
{
struct packet;
class stratum_client;

class stratum_response
{
  public:
    stratum_response(const json &in);
    static stratum_response parse(std::string_view in);
    
    const int requestId() const { return id;}
    std::string_view error() const { return err;}
    std::string_view nonce() const { return nonceStr;}
    std::string_view method() const { return methodStr;}
    const json& rawJson() const { return raw;}
    const std::vector<std::string> params() const { return parsedParams; }
private:
    int id;
    std::string methodStr;
    std::string err;
    std::string nonceStr;
    std::vector<std::string> parsedParams;
    const json raw;
};

} // namespace stratum
