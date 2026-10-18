#include "MutexQueue.h"

template<typename T>
MutexQueue<T>::MutexQueue(int cap)
{
    if (cap <= 0)
    {
        throw std::invalid_argument("Invalid capacity");
    }
    capacity = cap;
    buffer = new T[capacity];
}

template<typename T>
bool MutexQueue<T>::push(T val)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (push_cursor - pop_cursor == capacity)
    {
        return false;
    }

    buffer[push_cursor % capacity] = val;
    push_cursor++;
    return true;
}

template<typename T>
bool MutexQueue<T>::pop(T& val)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (push_cursor == pop_cursor)
    {
        return false;
    }

    val = buffer[pop_cursor % capacity];
    pop_cursor++;
    return true;
}

template<typename T>
size_t MutexQueue<T>::size()
{
    std::lock_guard<std::mutex> lock(mtx);
    return push_cursor - pop_cursor;
}

template<typename T>
MutexQueue<T>::~MutexQueue()
{
    delete[] buffer;
}

template class MutexQueue<int>;