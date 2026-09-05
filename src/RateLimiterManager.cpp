//
// Created by Bibenpreet Cheema on 9/2/26.
//

#include "../include/RateLimiterManager.h"
#include <vector>


RateLimiterManager::RateLimiterManager(int cap)
    :default_capacity(cap), stop_signal(), cleanup_thread(&RateLimiterManager::background_cleanup_loop,this)
{}


bool RateLimiterManager::allow_request(const std::string &client_id)
{
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
    auto[it, inserted] = TokenBuckets.try_emplace(client_id, nullptr);
    if (inserted) {
        it->second = std::make_unique<TokenBucket>(default_capacity);
    }

    return it->second-> request();

}

void RateLimiterManager::clean_inactive_buckets(std::chrono::seconds max_idle_time)
{
    auto now = std::chrono::steady_clock::now();
    //Cleaning inactive IDs
    std::vector<std::string> stale_id;
    //Read part->Filtering stale keys and storing them in vector
    // Scope of read lock
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
    if (!stale_id.empty())
    {
        std::unique_lock<std::shared_mutex> write_lock(map_mtx);
        auto now_check = std::chrono::steady_clock::now();
        for (const auto&key: stale_id) {
            auto it = TokenBuckets.find(key);
            if (it != TokenBuckets.end()) {
                if (now_check - it->second->get_last_accessed_time() > max_idle_time) {
                    TokenBuckets.erase(key);
                }
            }
        }
    }
}

void RateLimiterManager::background_cleanup_loop() {
    while (!stop_signal.load()) {
        std::unique_lock<std::mutex> lock(cv_mtx);
        cv.wait_for(lock,std::chrono::seconds(10), [this] () {return stop_signal.load();});
        if (!stop_signal.load()) {
            clean_inactive_buckets();
        }
    }
}

RateLimiterManager::~RateLimiterManager() {
    stop_signal.store(true);
    cv.notify_all();
    if (cleanup_thread.joinable()) {
        cleanup_thread.join();
    }
}

