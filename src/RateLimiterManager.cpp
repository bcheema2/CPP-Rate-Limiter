//
// Created by Bibenpreet Cheema on 9/2/26.
//

#include "../include/RateLimiterManager.h"
#include <vector>


RateLimiterManager::RateLimiterManager(int cap)
    :default_capacity(cap)
{}

bool RateLimiterManager::allow_request(const std::string &client_id) {
    //Read lock part
    {
        std::shared_lock<std::shared_mutex> read_lock(map_mtx);
        auto it = TokenBuckets.find(client_id);
        if (it != TokenBuckets.end()) {
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

void RateLimiterManager::clean_inactive_buckets(std::chrono::seconds max_idle_time) {
    auto now = std::chrono::steady_clock::now();
    //Cleaning inactive IDs
    std::vector<std::string> stale_id;
    //Read part->Filtering stale keys and storing them in vector

    {
        std::shared_lock<std::shared_mutex>read_lock(map_mtx);
        auto it = TokenBuckets.begin();

        while (it != TokenBuckets.end()) {
            auto stale = now - it->second->get_last_accessed_time();
            if (stale > max_idle_time ) {
                stale_id.push_back(it->first);
            }
            ++it;
        }
    }

    //Removing Stale IDs
    if (stale_id.empty()) {
        return;
    }
    else {
        std::unique_lock<std::shared_mutex> write_lock(map_mtx);
        for (const auto&key: stale_id) {
            TokenBuckets.erase(key);
        }
    }


}

