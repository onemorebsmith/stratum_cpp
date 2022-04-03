#pragma once

#include "types.h"

#include "socket.h"
#include "stratum_request.h"

namespace stratum
{

using callback_t = std::function<void(const stratum_client &client)>;

struct stratum_config
{
    std::string host;
    std::string userAgent;
    std::string user;
    std::string pass;
    int port;
};

class stratum_client
{
  public:
    static std::unique_ptr<stratum_client> NewClient(const stratum_config& params);
    ~stratum_client();

    bool begin();
    void setCallback(CallbackTypeE type, callback_t &&cb);
    bool execRequest(RequestHandle req);


  private:
    friend class stratum_protocol;

    stratum_client(const stratum_config& params);
    void dispatchCallback(CallbackTypeE type) const;

    std::unique_ptr<stratum_protocol> protocol;
};

} // namespace stratum