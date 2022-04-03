#include "stratum_response.h"

#include "stratum.h"

namespace stratum
{
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
    catch (json::exception)
    {
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
    fetch("id", in, id);
    fetch("error", in, err);

    json::array_t results = {};
    if (fetch_array("result", in, results))
        {
            if (std::size(results) > 1)
            {
                nonceStr = results[1].get<std::string>();
            }
        }
}
} // namespace stratum