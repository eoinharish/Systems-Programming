#include "SPSC.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <thread>
#include <vector>
#include <sched.h>

constexpr size_t TOTAL_MESSAGES = 1'000'000; // 1M
constexpr size_t CAPACITY = 1024;

void pin_to_cpu(std::thread& t, int core_id)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core_id, &set);

    int rc = pthread_setaffinity_np(t.native_handle(), sizeof(set), &set);
    if (rc != 0)
    {
        std::cerr << "CRITICAL: Failed to pin thread to core " << core_id << ", error code: " << rc << "\n";
    }
    else
    {
        std::cout << "Successfully pinned thread to core " << core_id << "\n";
    }
}

int main()
{
    SPSC<uint64_t> q(CAPACITY);

    std::vector<uint64_t> latencies(TOTAL_MESSAGES);

    std::atomic<bool> start_flag{false}; // // for simulatenous start

    std::thread producer([&]() {

        while (!start_flag.load(std::memory_order_acquire)) // for simulatenous start
        {

        }

        auto next = std::chrono::steady_clock::now();

        for (size_t i=0; i < TOTAL_MESSAGES; i++)
        {
            next += std::chrono::nanoseconds(1000); 

            while (std::chrono::steady_clock::now() < next) { }

            auto now = std::chrono::steady_clock::now();
            uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

            // auto start = std::chrono::steady_clock::now(); //
            // auto startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count();

            // Spin-wait if the queue is full
            while (!q.push(ts))
            {
                // std::this_thread::yield();
            }
        }
    });

    std::thread consumer([&]() {

        size_t received = 0;
        uint64_t startTime = 0;

        while (!start_flag.load(std::memory_order_acquire)) // for simulatenous start
        {

        }

        while (received < TOTAL_MESSAGES)
        {
            if (q.pop(startTime))
            {
                auto end = std::chrono::steady_clock::now();
                auto endTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end.time_since_epoch()).count();

                latencies[received] = endTime - startTime;
                received++;
            }
            else
            {
                // Yield or spin-wait briefly when empty
                // std::this_thread::yield();
            }
        }
    });

    pin_to_cpu(producer, 1);
    pin_to_cpu(consumer, 3);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    start_flag.store(true, std::memory_order_release); // Kickoff the benchmark

    producer.join();
    consumer.join();

    if (latencies.empty())
    {
        std::cout << "No samples collected.\n";
        return 1;
    }
    // before sorting
    for (size_t i = 0; i < latencies.size(); i++) {
        if (latencies[i] > 5000) {
            printf("idx %zu: %lu ns\n", i, latencies[i]);
        }
    }

    std::sort(latencies.begin(), latencies.end());

    auto get_percentile = [&](double p) {
        size_t index = static_cast<size_t> (p / 100 * latencies.size());
        return latencies[index];
    };

    std::cout << "=== SPSC Queue Latency Report (" << TOTAL_MESSAGES << " messages) ===\n";
    std::cout << "p50:   " << get_percentile(50.0) << " ns\n";
    std::cout << "p99:   " << get_percentile(99.0) << " ns\n";
    std::cout << "p99.9: " << get_percentile(99.9) << " ns\n";
    std::cout << "Max:   " << latencies.back() << " ns\n";

    return 0;
}