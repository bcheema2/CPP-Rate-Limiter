Markdown

# High-Peformance Multi-Threaded Rate Limiter

A thread-safe, low-latency rate limiter implemented in modern C++(C++20). This system uses a **Token Bucket** algorithm managed by a thread-safe registry (`RateLimiterManager`), featuring fine-grained concurrency control, automatic background memory reclamation for idle clients, and unit test coverage via GoogleTest.

---

## Key Technical Features

- **Token Bucket Algorithm:** Provides burst control and predictable token refill rates using standard `std::chrono::steady_clock` time intervals.
- **Concurrent Read/Write Access:** Utilizes `std::shared_mutex` to enable parallel reads for existing client lookups via `std::shared_lock`, acquiring an exclusive `std::unique_lock` only during new client bucket allocations or cleanup operations.
- **Lock-Free Metrics:** Tracks per-bucket allowed/blocked request counters using `std::atomic<uint64_t>` with `std::memory_order_relaxed` to minimize synchronization overhead inside critical paths.
- **Background Cleanup Daemon:** Spawns a dedicated background worker thread managing automatic garbage collection of stale/idle client buckets using `std::condition_variable_any` and a double-checked locking mechanism to prevent race conditions during deletion.
- **Unit Tested:** Integrated test suite built with GoogleTest (`gtest`) to verify bucket consumption bounds and system safety.

---

## File Structure

- **`include/TokenBucket.h` & `src/TokenBucket.cpp`**: Core rate-limiting bucket enforcing per-client capacities, lock-protected token replenishment, and atomic metrics tracking.
- **`include/RateLimiterManager.h` & `src/RateLimiterManager.cpp`**: High-level map managing individual `TokenBucket` instances mapped to `client_id`s, concurrent access control, and idle-bucket background cleanup execution.
- **`main.cpp`**: Simulation harness demonstrating multi-threaded burst workloads, atomic counter validation, and worker cleanup routines.
- **`tests/`**: GoogleTest suite for unit-level verification , Background Daemon & Automated Eviction , Multi-Threaded Lock Contention, Destructor & RAII Lifecycle Test.

---

## Build and Run Instructions

### Prerequisites

- **C++ Compiler:** Supporting C++20 standard (GCC 10+, Clang 11+, or MSVC 2019+)
- **Build System:** CMake 3.20+
- **Unit Testing Framework:** GoogleTest (`gtest`)