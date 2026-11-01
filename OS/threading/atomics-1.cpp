#include <iostream>
#include <atomic>
#include <string>
#include <atomic>

std::atomic<int> counter{0};

int main()
{
    std::atomic<int> x{0};

    // Atomic exchange (atomic swap)
    // returns the old value and writes the new value
    int old = x.exchange(2); // xchg
    std::cout << "old: " << old << " x: " << x << '\n';

    // read x, add to x, return old value
    int old1 = x.fetch_add(3); // lock xadd
    std::cout << "old1: " << old1 << " x: " << x << '\n';

    int expected = 4;
    int desired = 10;

    // if x == expected then modify x to desired, else don't do anything
    bool flag = x.compare_exchange_strong(expected, desired); // lock cmpxchg
    std::cout << "flag: " << flag << " x: " << x << '\n';

    return 0;
}