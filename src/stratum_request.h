#pragma once

#include "types.h"

namespace stratum
{
struct packet;
class stratum_client;
class stratum_response;

using request_callback_t = std::function<bool(const stratum_response &)>;

class stratum_request
{
  public:
    stratum_request(stratum_client &c, const std::string &method);

    template <typename... params_t>
    stratum_request(stratum_client &c, const std::string &method, params_t &&...params) : stratum_request(c, method)
    {
        paramArray = nlohmann::json::array_t{std::forward<params_t>(params)...};
    }

    stratum_request &then(request_callback_t _handler) &
    {
        handler = _handler;
        return *this;
    }
    stratum_request &&then(request_callback_t _handler) &&
    {
        handler = _handler;
        return std::move(*this);
    }

    std::string toString() const;
    const int requestId() const
    {
        return id;
    }

    void dispatch(const stratum_response &response) const;

  private:
    const int id;
    const std::string method;
    nlohmann::json::array_t paramArray;
    request_callback_t handler;
    stratum_client &client;
};

using RequestHandle = std::shared_ptr<stratum_request>;

} // namespace stratum
