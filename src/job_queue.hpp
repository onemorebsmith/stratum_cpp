#pragma once
#include <mutex>
#include <map>

#include "stratum_request.h"

namespace stratum
{
class job_queue
{
    std::mutex queueLock;
    std::map<int, RequestHandle> jobs;
    public:
    void enqueue(RequestHandle req)
    {
        std::scoped_lock g(queueLock);
        jobs.insert({req->requestId(), req});
    }

    RequestHandle dispatch(int jobId)
    {
        RequestHandle job = nullptr;
        std::scoped_lock g(queueLock);
        const auto found = jobs.find(jobId);
        if (found != std::end(jobs))
        {
            job = found->second;
            jobs.erase(found);
        }
        return job;
    }
};
} // namespace stratum