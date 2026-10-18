#include "SPSC.h"
#include <thread>
#include <functional>


void producer(SPSC<int>& q, atomic<int>& fullRetries)
{
    for(int i=0; i < 1'000'000; )
    {
        if (q.push(i))
        {
            i++;
        }
        else
        {
            fullRetries++;
            std::this_thread::yield();
        }
    }

}

void consumer(SPSC<int>& q, atomic<int>& emptyRetries, atomic<bool>& failed, atomic<int>& consumed)
{
    for(int expected = 0; expected < 1'000'000; )
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

int main()
{
    SPSC<int> q(64);

    atomic<int> fullRetries{0};
    atomic<int> emptyRetries{0};
    atomic<int> consumed{0};
    atomic<bool> failed{false};

    std::thread t1(producer, std::ref(q), std::ref(fullRetries));
    std::thread t2(consumer, std::ref(q), std::ref(emptyRetries), std::ref(failed), std::ref(consumed));

    t1.join();
    t2.join();

    cout << "Full retries: " << fullRetries.load() << '\n';
    cout << "Empty retries: " << emptyRetries.load() << '\n';
    cout << "Consumed: " << consumed.load() << '\n';
    cout << "failed: " << failed.load() << '\n';
}