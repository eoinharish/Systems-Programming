#include "SPSC.h"
#include "MutexQueue.h"
#include <thread>
#include <functional>
#include <chrono>
#include <vector>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sched.h>
#include <new>
#include <atomic>

constexpr int NUM_ITEMS = 10'000'000;
constexpr int REPS = 5;

static void pin_to_cpu(int cpu)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    if (rc != 0)
    {
        std::fprintf(stderr, "pin_to_cpu(%d) failed: %s\n", cpu, std::strerror(rc));
        std::exit(1);   // for a benchmark, a failed pin makes results invalid
    }
}


template<typename Q>
void producer(Q& q, size_t& fullRetries, std::atomic<bool>& failed)
{
    pin_to_cpu(0);
    for(int i=0; i < NUM_ITEMS; )
    {
        if (q.push(i))
        {
            i++;
        }
        else
        {
            if (failed.load()) return;   // avoids hanging if consumer bailed out
            fullRetries++;

            // std::this_thread::yield() hints to the thread scheduler that the current thread is willing to let another
            // runnable thread execute.
            // It doesn't block or sleep the thread. The scheduler may run another thread or immediately resume the current one.
            std::this_thread::yield(); // let other thread progress (but depends on thread scheduler)
        }
    }
}

template<typename Q>
void consumer(Q& q, size_t& emptyRetries, std::atomic<bool>& failed, size_t& consumed)
{
    pin_to_cpu(1);
    for(int expected = 0; expected < NUM_ITEMS; )
    {
        int val;
        if (q.pop(val))
        {
            if (val != expected)
            {
                std::cerr << "ERROR: expected " << expected << ", got " << val << '\n';
                failed.store(true);
                return;
            }
            expected++;
            consumed++;
        }
        else
        {
            emptyRetries++;
            std::this_thread::yield();
        }
    }
}

struct Result
{
    double mops;      // million items/sec
    size_t fullRetries;
    size_t emptyRetries;
    bool failed;
};

template<typename Q>
Result run(int cap)
{
    Q q(cap);

    alignas(cache_line_size) size_t fullRetries{0};
    alignas(cache_line_size) size_t emptyRetries{0};
    alignas(cache_line_size) size_t consumed{0};
    alignas(cache_line_size) std::atomic<bool> failed{false};

    auto start = std::chrono::steady_clock::now();

    std::thread t1(producer<Q>, std::ref(q), std::ref(fullRetries), std::ref(failed));
    std::thread t2(consumer<Q>, std::ref(q), std::ref(emptyRetries), std::ref(failed), std::ref(consumed));

    t1.join();
    t2.join();

    auto end = std::chrono::steady_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();

    bool bad = failed.load() || consumed != NUM_ITEMS;
    return { NUM_ITEMS / sec / 1e6, fullRetries, emptyRetries, bad };
}

// Run REPS times, return the median result by throughput
template<typename Q>
Result median_run(int cap)
{
    std::vector<Result> results;
    for (int i = 0; i < REPS; i++)
    {
        results.push_back(run<Q>(cap));
    }
    std::sort(results.begin(), results.end(),
              [](const Result& a, const Result& b){ return a.mops < b.mops; });
    return results[results.size() / 2];
}

int main()
{
    std::printf("Items per run: %d, repetitions: %d (median reported)\n\n", NUM_ITEMS, REPS);

    for (int cap : {64, 1024, 65536})
    {
        Result mtx = median_run<MutexQueue<int>>(cap);
        Result lf  = median_run<SPSC<int>>(cap);

        std::printf("cap=%-6d\n", cap);
        std::printf("  mutex     : %7.2f M items/s  (full retries: %zu, empty retries: %zu, failed: %d)\n",
                    mtx.mops, mtx.fullRetries, mtx.emptyRetries, (int)mtx.failed);
        std::printf("  lock-free : %7.2f M items/s  (full retries: %zu, empty retries: %zu, failed: %d)\n",
                    lf.mops, lf.fullRetries, lf.emptyRetries, (int)lf.failed);
        std::printf("  speedup   : %.2fx\n\n", lf.mops / mtx.mops);
    }
}