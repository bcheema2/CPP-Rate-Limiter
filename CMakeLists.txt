cmake_minimum_required(VERSION 4.3)
project(Rate_Limiter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rate_Limiter main.cpp
        TokenBucket.h
        TokenBucket.cpp
        RateLimiterManager.cpp
        RateLimiterManager.h)
