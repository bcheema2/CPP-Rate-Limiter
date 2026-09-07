//
// Created by Bibenpreet Cheema on 9/6/26.
//
#include <gtest/gtest.h>
#include "RateLimiterManager.h"
#include <string>
#include <atomic>
#include <chrono>
#include<thread>


//TEST 1
TEST(RateLimiterManagerTest, simulate_test_1) {

    RateLimiterManager manager(3);
    std::string client = "test_user";

    for (int i = 0; i < 3; i++) {
        EXPECT_TRUE(manager.allow_request(client));
    }
    EXPECT_FALSE(manager.allow_request(client));
}

//TEST 2 (Background Daemon & Automated Eviction)
TEST(RateLimiterManagerTest, simulate_test_2) {
    RateLimiterManager manager(3);
    std::string client = "test_user";
    for (int i = 0; i < 3; i++) {
        manager.allow_request(client);
    }
    manager.clean_inactive_buckets(std::chrono::seconds(0));
    EXPECT_TRUE(manager.allow_request(client));
}

//TEST 3 (Multi-Threaded Lock Contention)
TEST(RateLimiterManagerTest, simulate_test_3) {
    RateLimiterManager manager(10);
    std::string client = "heavy_user";
    std::atomic<int> pass_count{0}, block_count{0};
    std::vector<std::thread> workers;

    for (int i = 0; i < 4;i++) {
        workers.emplace_back([&] () {
            for (int j = 0; j < 10;j++) {
                if (manager.allow_request(client)) {
                    pass_count++;
                }
                else {
                    block_count++;
                }
            }
        });
    }

    for (auto& w : workers) {
        w.join();
    }
    EXPECT_EQ(pass_count.load(),10);
    EXPECT_EQ(block_count.load(),30);
}

//TEST 4 (Destructor & RAII Lifecycle Test)
TEST(RateLimiterManagerTest, simulate_test_4) {
    EXPECT_NO_THROW({
        for (int i = 0; i < 5; i ++) {
            RateLimiterManager manager(5);
            manager.allow_request("test_user");
        }
    });
}