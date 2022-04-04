#pragma once

#include "stratum_client.h"

#include "job_queue.hpp"
#include "logger.h"
#include "stratum_response.h"
#include "types.h"
#include <thread>

namespace stratum
{
class stratum_protocol
{
  private:
    static constexpr auto userAgent = "test_agent/v0.1";
    SocketHandle socket;
    const stratum_config config;
    uint nonce;
    bool running;
    std::thread dispatchThread;
    job_queue jobQueue;
    logger logger;
    stratum_client &client;

    // callbacks
    callback_t callbacks[static_cast<int>(CallbackTypeE::CALLBACKTYPE_LAST)];
    mining_notify_cb_t miningNotifyCb;

  public:
    stratum_protocol(const stratum_config &_config, stratum_client &_client)
        : client(_client), config(_config), logger{"[protocol]"}
    {
    }

    bool begin()
    {
        stop(); // cleanup anything we were already doing

        socket = tcp_socket::Open(config.host, config.port);
        logger.info() << "connecting to host " << config.host << ":" << config.port;
        socket->connect();

        running = true;
        startDispatchThread();
        return doHandshake();
    }

    void stop()
    {
        socket = nullptr;
        running = false;
        if (dispatchThread.joinable())
        {
            dispatchThread.join();
        }
    }

    bool doHandshake()
    {
        if (!socket->connected())
        {
            logger.info() << "connecting to host " << config.host << ":" << config.port;
            socket->connect();
        }

        auto req = std::make_shared<stratum_request>(client, "mining.subscribe", userAgent, nullptr,
                                                     config.host.c_str(), std::to_string(config.port));
        req->then([&](const stratum_response &response) {
            client.dispatchCallback(CallbackTypeE::CALLBACKTYPE_HANDSHAKE);
            doAuth();
            return true;
        });

        logger.debug() << "sending handshake: " << req->toString();
        client.execRequest(req);
        return true;
    }

    bool doAuth()
    {
        auto req = std::make_shared<stratum_request>(client, "mining.authorize", config.user, config.pass);
        req->then([&](const stratum_response &response) {
            client.dispatchCallback(CallbackTypeE::CALLBACKTYPE_AUTH);
            doSubscribe();
            return true;
        });
        logger.debug() << "sending auth: " << req->toString();
        client.execRequest(req);

        return true;
    }

    bool doSubscribe()
    {
        auto req = std::make_shared<stratum_request>(client, "mining.extranonce.subscribe");
        req->then([&](const stratum_response &response) {
            client.dispatchCallback(CallbackTypeE::CALLBACKTYPE_SUBSCRIBE);
            return true;
        });
        logger.debug() << "sending subscribe: " << req->toString();
        client.execRequest(req);

        return true;
    }

    void startDispatchThread()
    {
        dispatchThread = std::thread([this] {
            auto dispatchLogger = stratum::logger("[dispatch]");
            while (running)
            {
                if (socket->connected() == false)
                {
                    dispatchLogger.debug() << "not connected to host, waiting";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }

                const auto packet = socket->Receive();
                if (packet.has_value() == false)
                {
                    dispatchLogger.info() << "connection to host lost";
                    break; // socket died
                }
                auto response = std::stringstream(packet->get()->data().data()); // needs more data
                std::string line;
                while (std::getline(response, line))
                {
                    dispatchLogger.debug() << "received: " << line;
                    const auto resp = stratum_response::parse(line);
                    auto job = jobQueue.dispatch(resp.requestId());
                    if (job != nullptr)
                    {
                        job->dispatch(resp);
                    }
                    else
                    {
                        // no job so unsolicited/broadcast/notify/etc..
                        handleNotification(resp);
                    }
                }
            }

            return true;
        });
    }

    void handleNotification(const stratum_response &resp)
    {
        if (resp.method() == "mining.notify")
        {
            if (miningNotifyCb != nullptr)
            {
                auto params = mining_notify{ .params = resp.params() };
                miningNotifyCb(client, params);
            }
        }
    }

    bool execRequest(RequestHandle req)
    {
        jobQueue.enqueue(req);
        const auto serialized = req->toString();
        return send(serialized, serialized.length());
    }

    bool send(std::string_view data, size_t sz)
    {
        auto success = false;
        if (socket->connected())
        {
            logger.debug() << "sending: " << data;
            success = socket->send((void *)data.data(), sz);
            if (!success)
            {
                logger.err() << "error sending to host " << config.host << ":" << config.port;
            }
        }
        return success;
    }

    void setCallback(CallbackTypeE type, callback_t &&cb)
    {
        if (type >= CallbackTypeE::CALLBACKTYPE_FIRST && type < CallbackTypeE::CALLBACKTYPE_LAST)
        {
            const auto idx = static_cast<int>(type);
            callbacks[idx] = std::forward<decltype(cb)>(cb);
        }
    };

    void dispatchCallback(CallbackTypeE type) const
    {
        if (type >= CallbackTypeE::CALLBACKTYPE_FIRST && type < CallbackTypeE::CALLBACKTYPE_LAST)
        {
            const auto idx = static_cast<int>(type);
            auto &cb = callbacks[idx];
            if (cb != nullptr)
            {
                cb(client);
            }
        }
    }

    void onMiningNotify(mining_notify_cb_t &&cb)
    {
        miningNotifyCb = std::forward<mining_notify_cb_t>(cb);
    }

    void join(){
        if (dispatchThread.joinable()){
            dispatchThread.join();
        }
    }
  private:
    static void log(std::string_view msg)
    {
        std::cout << "[StratumProtocol] " << msg.data() << std::endl;
    }
};

} // namespace stratum