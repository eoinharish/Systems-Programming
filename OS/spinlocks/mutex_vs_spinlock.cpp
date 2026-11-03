#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <benchmark/benchmark.h>
#include <immintrin.h>

class Spinlock
{
    private:
        std::atomic<bool> flag {false}; // False -> unlocked

    public:
        void lock()
        {
            while (1)
            {
                if (!flag.exchange(true, std::memory_order_acquire)) // test-and-set
                {
                    return;
                }

                while (flag.load(std::memory_order_relaxed)) // test
		{
		    // read-only spinning
		    _mm_pause(); // x86 PAUSE instruction: rep nop
		}
                
            }
        }

        void unlock()
        {
            flag.store(false, std::memory_order_release);
        }
};

int counter {0};
std::mutex mtx; // global shared mutex
Spinlock sl; // global shared spinlock

static void BM_mutex(benchmark::State& s)
{
    for(auto _ : s)
    {
        mtx.lock();

        counter++;
        for (volatile int i = 0; i < 100; i++){}
        
	mtx.unlock();
    }

    benchmark::DoNotOptimize(counter); // avoid dead-code elimination of counter
}

static void BM_spinlock(benchmark::State& s)
{
    for(auto _ : s)
    {
        sl.lock();
        
	counter++;
	for (volatile int i = 0; i < 100; i++){}
        
	sl.unlock();
    }

    benchmark::DoNotOptimize(counter); // avoid dead-code elimination of counter
}


BENCHMARK(BM_mutex)
	->Threads(1)
	->Threads(2)
	->Threads(4)
	->Threads(8);

BENCHMARK(BM_spinlock)
	->Threads(1)
	->Threads(2)
	->Threads(4)
	->Threads(8);

BENCHMARK_MAIN();





