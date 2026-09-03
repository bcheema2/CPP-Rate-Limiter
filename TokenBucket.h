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
    std::chrono::steady_clock::time_point last_refill;

    //Atomic metric counters rather then using std::cout
    std::atomic<uint64_t> total_allowed{0};
    std::atomic<uint64_t> total_denied{0};

    void refill(int amount = 1);
public:


    TokenBucket(int cap);

    bool request();

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
