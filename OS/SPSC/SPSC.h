#pragma once
#include <iostream>
#include <atomic>
#include <memory>
#include <new>

#ifdef NO_ALIGN
    #define CURSOR_ALIGN
#else
    #define CURSOR_ALIGN alignas(cache_line_size)
#endif

constexpr size_t cache_line_size  = std::hardware_destructive_interference_size;

template<typename T>
class SPSC
{
    public:
        T* buffer;
        size_t capacity;
        CURSOR_ALIGN std::atomic<size_t> pop_cursor {0};
        CURSOR_ALIGN std::atomic<size_t> push_cursor {0};

    public:
        SPSC(int cap = 10);

        // std::atomic is neither copyable nor movable
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