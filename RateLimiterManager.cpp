//
// Created by Bibenpreet Cheema on 9/2/26.
//

#include "RateLimiterManager.h"

RateLimiterManager::RateLimiterManager(int cap)
    :default_capacity(cap)
{}

bool RateLimiterManager::allow_request(const std::string &client_id) {
    //Read lock part
    {
        auto it = TokenBuckets.find(client_id);
        std::shared_lock<std::shared_mutex> read_lock(map_mtx);
        if (it == TokenBuckets.end()) {
            return it->second->request();
        }

    }
    //read_lock goes out of scope here.
    //If client_id is not present, write_lock lock get exclusive access to thread.
    std::unique_lock<std::shared_mutex> write_lock(map_mtx);
    if (!TokenBuckets.contains(client_id)) {
        TokenBuckets.emplace(client_id, std::make_unique<TokenBucket>(default_capacity));
    }

    return TokenBuckets[client_id]-> request();

}

