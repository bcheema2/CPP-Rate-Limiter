//
// Created by Bibenpreet Cheema on 9/6/26.
//
#include <gtest/gtest.h>
#include "RateLimiterManager.h"
#include <string>

TEST(RateLimiterManagerTest, simulate_test_1) {

    RateLimiterManager manager(3);
    std::string client = "test_user";

    for (int i = 0; i < 3; i++) {
        EXPECT_TRUE(manager.allow_request(client));
    }
    EXPECT_FALSE(manager.allow_request(client));
}

