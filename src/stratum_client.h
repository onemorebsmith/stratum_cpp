#pragma once

#include "types.h"

#include "socket.h"
#include "stratum_request.h"

namespace stratum
{

using callback_t = std::function<void(const stratum_client &client)>;
using client_handle_t = std::unique_ptr<stratum_client>;

struct mining_notify{
    const std::vector<std::string>& params;
};

using mining_notify_cb_t = std::function<void(const stratum_client &client, const mining_notify& params)>;

struct stratum_config
{
    std::string host;
    int port;
    std::string userAgent;
    std::string user;
    std::string pass;
};

class stratum_client
{
  public:
    static std::unique_ptr<stratum_client> newClient(const stratum_config& params);
    ~stratum_client();

    bool begin();
    void setCallback(CallbackTypeE type, callback_t &&cb);
    void onMiningNotify(mining_notify_cb_t &&cb);


    bool execRequest(RequestHandle req);

    void join();
  private:
    friend class stratum_protocol;

    stratum_client(const stratum_config& params);
    void dispatchCallback(CallbackTypeE type) const;

    std::unique_ptr<stratum_protocol> protocol;
};

} // namespace stratum