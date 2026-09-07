//
// Created by Bibenpreet Cheema on 9/2/26.
//

#ifndef RATE_LIMITER_RATELIMITERMANAGER_H
#define RATE_LIMITER_RATELIMITERMANAGER_H

#include "TokenBucket.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>


class RateLimiterManager
{
    private:
        mutable std::shared_mutex map_mtx;
        std::unordered_map<std::string, std::unique_ptr<TokenBucket>> TokenBuckets;
        int default_capacity;
        std::atomic<bool> stop_signal{false};
        std::mutex cv_mtx;
        std::condition_variable_any cv;
        std:: thread cleanup_thread;
        void background_cleanup_loop();

    public:
        RateLimiterManager(int capacity);
        ~RateLimiterManager();

        bool allow_request(const std::string& client_id);

        void clean_inactive_buckets(std::chrono::seconds max_idle_time = std::chrono::seconds(10));
};


#endif //RATE_LIMITER_RATELIMITERMANAGER_H
