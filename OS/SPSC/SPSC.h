#include <iostream>
#include <atomic>
#include <memory>
#include <new>

using namespace std;

inline constexpr size_t cache_line_size  = std::hardware_destructive_interference_size;

template<typename T>
class SPSC
{
    public:
        T* buffer;
        size_t capacity;
        alignas(cache_line_size) atomic<size_t> pop_cursor {0};
        alignas(cache_line_size) atomic<size_t> push_cursor {0};

    public:
        SPSC(int cap = 10);

        SPSC(const SPSC& other) = delete;
        SPSC& operator=(const SPSC& other) = delete;
        SPSC(SPSC&& other) = delete;
        SPSC& operator=(SPSC&& other) = delete;
        
        bool push(T val);

        bool pop(T& val);

        size_t size();

        bool isPushCursorLockFree();
        bool isPopCursorLockFree();

        ~SPSC();

};