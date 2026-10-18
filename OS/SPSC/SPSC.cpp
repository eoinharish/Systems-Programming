#include "SPSC.h"

template<typename T>
SPSC<T>::SPSC(int cap)
{
    capacity = cap;

    if (capacity <= 0)
    {
        throw std::invalid_argument("Invalid capacity");
    }
    buffer = new T[capacity];
}

template<typename T>
bool SPSC<T>::push(T val)
{
    size_t pushCursor = push_cursor.load(std::memory_order_relaxed);
    size_t popCursor = pop_cursor.load(std::memory_order_acquire);

    if (pushCursor - popCursor == capacity)
    {
        return false;
    }

    buffer[pushCursor % capacity] = val;
    
    push_cursor.store(pushCursor + 1, std::memory_order_release);
    return true;
}

template<typename T>
bool SPSC<T>::pop(T& val)
{
    size_t popCursor = pop_cursor.load(std::memory_order_relaxed);
    size_t pushCursor = push_cursor.load(std::memory_order_acquire);

    if (pushCursor == popCursor)
    {
        return false;
    }
    val = buffer[popCursor % capacity];
    pop_cursor.store(popCursor + 1, std::memory_order_release);
    return true;
}

template<typename T>
size_t SPSC<T>::size()
{
    size_t pushCursor = push_cursor.load(std::memory_order_relaxed);
    size_t popCursor = pop_cursor.load(std::memory_order_relaxed);
    return pushCursor - popCursor;
}

template<typename T>
bool SPSC<T>::isPushCursorLockFree()
{
    return push_cursor.is_lock_free();
}

template<typename T>
bool SPSC<T>::isPopCursorLockFree()
{
    return pop_cursor.is_lock_free();
}

template<typename T>
SPSC<T>::~SPSC()
{
    cout << "dtor()\n";
    delete[] buffer;
}

template class SPSC<int>;