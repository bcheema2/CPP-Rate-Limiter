//
// Created by Bibenpreet Cheema on 8/30/26.
//
#include "TokenBucket.h"
#include <iostream>
#include <algorithm>
using namespace std::chrono_literals;

TokenBucket::TokenBucket(int cap)
    : tokens(cap), capacity(cap), last_refill(std::chrono::steady_clock::now())
{}

void TokenBucket::refill(int amount) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = now - last_refill;

    //Calculating how many 1s interval have passed
    auto seconds_passed = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    if (seconds_passed >= 1) {
        int tokens_to_add = static_cast<int>(seconds_passed) * amount;
        tokens = std::min(capacity,tokens + tokens_to_add);
        last_refill += std::chrono::seconds(seconds_passed);
    }

}

bool TokenBucket::request() {
    bool allowed = false;

    {
        std::lock_guard<std::mutex> lock(mtx);
        refill();

        if (tokens >= 1) {
            tokens --;
            allowed = true;
        }
    } //Lock is released here

    //Increment atomic counters outside the lock
    if (allowed) {
        total_allowed.fetch_add(1, std::memory_order_relaxed);
    }
    else {
        total_denied.fetch_add(1, std::memory_order_relaxed);
    }
    return allowed;


}
