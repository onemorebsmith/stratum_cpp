#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdlib.h>
#include <string>
#include <memory>
#include <map>
#include <mutex>

#include <string_view>

namespace stratum
{
using json = nlohmann::json;

class stratum_request;
class stratum_response;
class stratum_protocol;
class stratum_client;

enum class CallbackTypeE
{
    CALLBACKTYPE_UNKNOWN,
    CALLBACKTYPE_HANDSHAKE,
    CALLBACKTYPE_AUTH,
    CALLBACKTYPE_NOTIFY,
    CALLBACKTYPE_SUBSCRIBE,
    CALLBACKTYPE_SET_TARGET,
    CALLBACKTYPE_LAST = CALLBACKTYPE_SET_TARGET + 1,
    CALLBACKTYPE_FIRST = CALLBACKTYPE_UNKNOWN
};

} // namespace stratum