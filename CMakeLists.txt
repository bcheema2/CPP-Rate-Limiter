cmake_minimum_required(VERSION 4.3)
project(Rate_Limiter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(include)

set(CORE_SOURCES
        src/TokenBucket.cpp
        src/RateLimiterManager.cpp
)

add_executable(Rate_Limiter
        src/main.cpp
        ${CORE_SOURCES}
)

# -----------------------------------------------------------------------------
# 2. Fetch Google Test from GitHub
# -----------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# -----------------------------------------------------------------------------
# 3. Test Runner Executable
# -----------------------------------------------------------------------------
add_executable(Rate_Limiter_Tests
        tests/RateLimiterManagerTest.cpp
        ${CORE_SOURCES}
)

# Link Google Test (which supplies main()) and C++ threading support
target_link_libraries(Rate_Limiter_Tests PRIVATE gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(Rate_Limiter_Tests)