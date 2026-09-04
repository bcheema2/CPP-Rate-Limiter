//
// Created by Bibenpreet Cheema on 8/30/26.
//
#pragma once

#ifndef RATE_LIMITER_TOKENBUCKET_H
#define RATE_LIMITER_TOKENBUCKET_H
#include <chrono>
#include<mutex>
#include<atomic>


class TokenBucket {
private:
    int tokens;
    int capacity;
    mutable std:: mutex mtx;
    //This is to refill Bucket
    std::chrono::steady_clock::time_point last_refill;
    // This is to track when a client_id was last accessed so
    // we can clean the bucket after set time.
    std::chrono::steady_clock::time_point last_accessed;

    //Atomic metric counters rather then using std::cout
    std::atomic<uint64_t> total_allowed{0};
    std::atomic<uint64_t> total_denied{0};

    void refill(int amount = 1);
public:


    TokenBucket(int cap);

    bool request();
    //Getter method to get last accessed time
    auto get_last_accessed_time() const {
        std::lock_guard<std::mutex> lock(mtx);
        return last_accessed;
    }

    //Getter method to get Tokens
    int get_tokens() const {
        std::lock_guard<std::mutex> lock(mtx);
        return tokens;
    }

    uint64_t get_allowed_count() const {
        return total_allowed.load(std::memory_order_relaxed);
    }
    uint64_t get_denied_count() const {
        return total_denied.load(std::memory_order_relaxed);
    }

};

#endif //RATE_LIMITER_TOKENBUCKET_H
