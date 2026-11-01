#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <benchmark/benchmark.h>

// Shared counter
int counter {0};
std::mutex mtx;

class Spinlock
{
    private:
        std::atomic<bool> flag {false}; // False -> unlocked

    public:
        void lock()
        {
            while (1)
            {
                if (!flag.exchange(true, std::memory_order_acq_rel)) // test-and-set
                {
                    return;
                }

                while (flag.load(std::memory_order_relaxed)); // test
                
            }
        }

        void unlock()
        {
            flag.store(false, std::memory_order_release);
        }
};

static void BM_mutex(benchmark::State& s)
{
    for(auto _ : s)
    {
        mtx.lock();
        counter++;
        mtx.unlock();
    }

    benchmark::DoNotOptimize(counter); // avoid dead-code elimination of counter
}

static void BM_spinlock(benchmark::State& s)
{
    Spinlock sl;

    for(auto _ : s)
    {
        sl.lock();
        counter++;
        sl.unlock();
    }

    benchmark::DoNotOptimize(counter); // avoid dead-code elimination of counter
}


BENCHMARK(BM_mutex);
BENCHMARK(BM_spinlock);
BENCHMARK_MAIN();





