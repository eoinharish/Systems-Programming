#include <iostream>
#include <atomic>
#include <thread>
#include <benchmark/benchmark.h>

class Spinlock
{
    private:
        std::atomic<bool> locked{false};

    public:
        void lock(){

            // atomic exchange returns the old value of the lock
            // If the lock is free (false), it is set to true and loop exits
            // If the lock is taken (true), spin in the loop until someone else frees the lock
            // and we grab it
            while(locked.exchange(true)){
            }
        }

        void unlock(){
            locked.store(false); // NOTE: revise seq_cst, relaxed, store-release
        }
};

void inc (Spinlock& sl, int64_t& val)
{
    for(int i=0; i < 100'000; i++)
    {
        sl.lock();
        val++;
        sl.unlock();
    }
}

static void naive(benchmark::State &s)
{
    auto num_threads = s.range(0);

    // allocate a vector of threads
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

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

BENCHMARK(naive)
    ->RangeMultiplier(2)
    ->Range(1, std::thread::hardware_concurrency()) // range[1, no_of_hardware_threads]
    ->UseRealTime()
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();

// Google Benchmark includes the upper bound of the range, even if the multiplier would skip over it.
// NOTE: ->RangeMultiplier(16)
//       ->Range(1, 4) -> runs naive/1, naive/4