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
#include <mutex>

class RateLimiterManager {
private:
    mutable std::shared_mutex map_mtx;
    std::unordered_map<std::string, std::unique_ptr<TokenBucket>> TokenBuckets;
    int default_capacity;

public:
    RateLimiterManager(int capacity);

    bool allow_request(const std::string& client_id);

    void clean_inactive_buckets(std::chrono::seconds max_idle_time = std::chrono::minutes(5));
};


#endif //RATE_LIMITER_RATELIMITERMANAGER_H
