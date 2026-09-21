#include <iostream>
#include <atomic>
#include <thread>
#include <benchmark/benchmark.h>

class Spinlock
{
    private:
        std::atomic<bool> locked{false};

    public:
        void lock()
        {
            while(1)
            {
                // Try and grab the lock
                // Return if we get the lock
                if(!locked.exchange(true, std::memory_order_acquire)){
                    return;
                }

                // If we didn't get the lock, just read the value which gets cached locally.
                // This minimizes the atomic writes-> leads to less cache-coherence traffic
                // and less no. of cache invalidations
                while(locked.load(std::memory_order_relaxed)){

                }
            }
        }

        void unlock(){
            locked.store(false, std::memory_order_release); // NOTE: revise seq_cst, relaxed, store-release
        }
};

void inc (Spinlock& sl, int64_t& val)
{
    for(int i=0; i < 100'000; i++)
    {
        // Hot path
        // Frequent atomic reads/writes
        // Atomic writes causes L1-d cache misses
        sl.lock();
        val++;
        sl.unlock();
    }
}

// Small benchmark
static void spin_locally(benchmark::State &s)
{
    // Sweep over a range of threads
    auto num_threads = s.range(0);

    // allocate a vector of threads
    std::vector<std::thread> threads;
    threads.reserve(num_threads); // To avoid repeated reallocations, refer reserve() vs resize() card

    Spinlock sl;
    int64_t val = 0; // value we will increment

    // timing loop for taking measurements
    for (auto _ : s)
    {
        // CREATE N THREADS
        for(auto i=0; i < num_threads; i++)
        {
            threads.emplace_back( [&](){
                inc(sl, val);
            });
        }

        // JOIN N THREADS
        for(auto& thread: threads)
        {
            thread.join();
        }

        // IMP: CLEAR thread vector
        // Else in the next iteration, it will create N additional threads and try to join threads which are non-joinable.
        // double join causes program termination, so best practice if(t.joinable()) { t.join();}
        threads.clear();
    }

}

BENCHMARK(spin_locally)
    ->RangeMultiplier(2)
    ->Range(1, std::thread::hardware_concurrency()) // range[1, no_of_hardware_threads]
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

// Google Benchmark includes the upper bound of the range, even if the multiplier would skip over it.
// NOTE: ->RangeMultiplier(16)
//       ->Range(1, 4) -> runs naive/1, naive/4