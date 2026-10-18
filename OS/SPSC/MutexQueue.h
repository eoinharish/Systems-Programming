#pragma once
#include <mutex>
#include <stdexcept>
#include <cstddef>

template<typename T>
class MutexQueue
{
    public:
        T* buffer;
        size_t capacity;
        size_t pop_cursor {0};
        size_t push_cursor {0};
        std::mutex mtx;

    public:
        MutexQueue(int cap = 10);

        MutexQueue(const MutexQueue& other) = delete;
        MutexQueue& operator=(const MutexQueue& other) = delete;
        MutexQueue(MutexQueue&& other) = delete;
        MutexQueue& operator=(MutexQueue&& other) = delete;

        bool push(T val);

        bool pop(T& val);

        size_t size();

        ~MutexQueue();
};