#include <iostream>
#include "RateLimiterManager.h"
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>


RateLimiterManager manager(10);
std::atomic<int> pass_count{0};
std::atomic<int> pass_denied{0};

void simulate_client(RateLimiterManager &mgr,const std::string &client_id,int num_request) {
    for (int i=0;i<num_request;i++) {
        if (mgr.allow_request(client_id)) {
            pass_count++;
        }
        else {
            pass_denied++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
int main() {
    std::cout << "--- Phase 1: High-Burst Multi-Threaded Load ---\n";

    std::vector<std::thread> workers;
    std::vector<std::string> clients = {"client_1","client_2","client_3","client_4"};

    for (const auto& client : clients ) {
        workers.emplace_back(simulate_client,std::ref(manager),client,20);
    }
    for (auto&t : workers) {
        t.join();
    }

    std::cout << "Phase 1 Complete!\n";
    std::cout << "Total Allowed Requests: " << pass_count.load() << "\n";
    std::cout << "Total Blocked Requests: " << pass_denied.load() << "\n";

    std::cout << "--- Phase 2: Testing Background Cleanup - Daemon ---\n";
    std::cout <<"Creating Temporary clients\n";

    //Temporary clients that will go idle
    manager.allow_request("client_A");
    manager.allow_request("client_B");

    std::cout << "Waiting for 10s\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Cleanup wait complete. System ready for shutdoen\n";


    return 0;
}
