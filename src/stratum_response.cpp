#include "stratum_response.h"

#include "logger.h"
#include "stratum.h"

namespace stratum
{
auto responseLogger = logger("[response]");

template <typename data_t> bool fetch(const char *key, const json &in, data_t &out)
{
    auto success = false;
    try
    {
        if (const auto ref = in.at(key); ref.is_null() == false)
        {
            ref.get_to(out);
            success = true;
        }
    }
    catch (json::exception e)
    {
        responseLogger.err() << "exception deserializing key: " << key << " " << e.what();
    }

    return success;
}

bool fetch_array(const char *key, const json &in, json::array_t &out)
{
    auto success = false;
    try
    {
        if (const auto ref = in.at(key); ref.is_null() == false && ref.is_array() == true)
        {
            out = ref.get<json::array_t>();
            success = true;
        }
    }
    catch (json::exception)
    {
    }

    return success;
}

stratum_response stratum_response::parse(std::string_view in)
{
    return stratum_response(json::parse(in.data()));
}

stratum_response::stratum_response(const json &in) : raw{in}, id{}, nonceStr{}, err{}
{
    auto idStr = std::string{};
    fetch("id", in, idStr);
    if (idStr != ""){ // TODO (bs): mybe actually bother with a try/catch here since stoi can throw
        id = std::stoi(idStr);
    }

    fetch("error", in, err);
    fetch("method", in, methodStr);

    json::array_t results = {};
    if (fetch_array("result", in, results))
    {
        if (std::size(results) > 1)
        {
            nonceStr = results[1].get<std::string>();
        }
    }
    json::array_t params = {};
    if (fetch_array("params", in, results))
    {
        for (const auto v : results){
            if (v.type() == json::value_t::string){
                parsedParams.push_back(v.get<std::string>());
            }
        }
    }
}
} // namespace stratum