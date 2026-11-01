#include <iostream>
#include <thread> // -std=c++11 or latest
#include <vector>
#include <atomic>

// Compile code with -fsanitize=thread and it will show exact data race

// int counter = 0; // Data race -> UB

// volatile int counter = 0; // can't fix data races -> still UB

// ATOMIC: data race-free, no UB, atomic read-modify-write (Hardware guarantees atomicity)
// Exact correct results, ie, if num_threads = 2 and each thread increments a counter 100 times
// then we'll get 2*100 = 200, i.e, no increments are lost due to concurrent updates.
// std::atomic<int> counter{0}; 

// x86 instruction	C++ atomic operation	            Purpose
//
// MOV	            load(), store()	                Atomic loads/stores for supported naturally aligned types
// XCHG	            exchange()	                    Atomic swap
// LOCK XADD	    fetch_add()	                    Atomic addition and returns old value
// LOCK CMPXCHG	    compare_exchange_weak/strong()	Compare-and-swap
// LOCK ADD	        Some atomic additions	        Atomic addition without needing old value
// MFENCE	        atomic_thread_fence(seq_cst)	Full memory fence

void inc_counter()
{
    // run loop ~1e4 times to notice UB with normal int counter
    for(int i=0; i<10000; i++)
    {
        counter++;
    }
}

int main()
{
    auto constexpr num_threads = 2;

    std::vector<std::thread> threads;
    threads.reserve(num_threads); // size = 0 capacity = 2

    for(auto i = 0; i < num_threads; i++)
    {
        threads.emplace_back(inc_counter);
    }

    for(auto& thread: threads) // "&" is reqd bcoz thread object is non-copyable (but movable)
    {
        thread.join();
    }

    std::cout << counter << '\n';

    return 0;
}