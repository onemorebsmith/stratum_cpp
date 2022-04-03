#pragma once

#include <string_view>
#include <iostream>
#include <sstream>

namespace util
{
    struct logger
    {
        struct logCtx
        {
            logCtx(std::string_view level, std::string_view prefix) : ss{}
            {
                ss << prefix << level << " ";
            }

            logCtx(logCtx &other) = delete;
            logCtx &operator=(logCtx &other) = delete;

            logCtx(logCtx &&other) : ss(std::move(other.ss))
            {
                std::cout << "moved" << std::endl;
            }
            logCtx &operator=(logCtx &&other)
            {
                this->ss = std::move(other.ss);
                return *this;
            };

            ~logCtx()
            {
                std::cout << ss.str() << std::endl;
            }

            template <typename data_t>
            constexpr logCtx &&operator<<(data_t &&d) &&
            {
                ss << std::forward<data_t &&>(d);
                return std::move(*this);
            }

            template <typename data_t>
            constexpr logCtx &operator<<(data_t &&d) &
            {
                ss << std::forward<data_t &&>(d);
                return *this;
            }

            std::stringstream ss;
        };

        logger(std::string_view _prefix) : prefix(_prefix) {}

        logCtx info() const
        {
            return logCtx("[info]", prefix);
        }

        logCtx err() const
        {
            return logCtx("[error]", prefix);
        }

        logCtx debug() const
        {
            return logCtx("[debug]", prefix);
        }

        const std::string prefix;
    };
}