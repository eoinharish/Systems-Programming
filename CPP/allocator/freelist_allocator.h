#pragma once

#include <iostream>

// ============================================================
// FREE-LIST ALLOCATOR (variable-size blocks, first-fit)
// ============================================================
//
// Every block (free or in-use) has a small Header right before it.
// Headers are chained together in ONE list covering the whole arena,
// in memory order (not just the free ones).
//
//   [Header][.....data.....][Header][.....data.....][Header]...
//
// allocate() : walk the list, find first FREE block that's big enough,
//              split it if there's a lot of leftover space.
// deallocate(): mark the block free, then try to merge with neighbors.


struct Header
{
    size_t size; // how big this block is (NOT including header)
    bool free; // is it currently in use or not

    Header* next; // pointer to the next block in memory, for walking the list
    Header* prev; // pointer to the previous block
};

class FreeListAllocator
{
    private:
        char* arena;
        Header* head; // first block in the whole arena

        size_t alignSize(size_t size)
        {
            size_t remainder = size % ALIGNMENT; // % 16

            if (remainder == 0)
            {
                return size;
            }

            size_t padding = ALIGNMENT - remainder;

            return size + padding;
        }

    public:
        static constexpr size_t ALIGNMENT = 16;
        static constexpr size_t HEADER_SIZE = sizeof(Header);

        FreeListAllocator(size_t totalSize)
        {
            if (totalSize < HEADER_SIZE + ALIGNMENT)
            {
                throw std::invalid_argument("Arena size too small");
            }

            arena = new char[totalSize];

            // the whole arena starts as ONE big free block
            head = reinterpret_cast<Header*>(arena);

            // Keep the block size aligned
            size_t availableBlockSize = totalSize - HEADER_SIZE;
            availableBlockSize = availableBlockSize - (availableBlockSize % ALIGNMENT);

            head->size = availableBlockSize;
            head->free = true;

            head->next = nullptr;
            head->prev = nullptr;
        }

        ~FreeListAllocator()
        {
            delete[] arena;
        }

        void* allocate(size_t requestedSize)
        {
            Header* current = head;

            // First-fit search: find the first free block big enough
            while(current != nullptr)
            {
                if(current->free && current->size >= requestedSize)
                {
                    split(current, requestedSize);
                    current->free = false;
                    
                    // Ex: block start address: 1000, return address: 1032
                    // As need to reserve 32 bytes for Header
                    return reinterpret_cast<char*>(current) + HEADER_SIZE;
                }
                current = current->next;
            }

            return nullptr; // no block big enough
        }

        void deallocate(void* ptr)
        {
            if (!ptr) return;

            Header* current = reinterpret_cast<Header*>(reinterpret_cast<char*>(ptr) - HEADER_SIZE);
            current->free = true;

            coalesce(current);
        }

        void debugPrint()
        {
            Header* current = head;

            int i = 0;
            while (current != nullptr)
            {
                std::cout << "Block " << i << ": size= " << current->size
                          << " free= " << current->free << '\n';
                current = current->next;
                i++;
            }
            std::cout << '\n';
        }

    private:

        // If 'block' is much bigger than 'requestedSize', cut off the extra
        // as a brand-new free block right after it.
        void split(Header* current, size_t requestedSize)
        {
            size_t leftover = current->size - requestedSize;

            if (leftover <= HEADER_SIZE)
            {
                return; // not worth splitting, just hand out the whole block
            }

            char* newBlockAddr = reinterpret_cast<char*>(current) + HEADER_SIZE + requestedSize;
            Header* newHeader = reinterpret_cast<Header*>(newBlockAddr);
            newHeader->size = current->size - (HEADER_SIZE + requestedSize); // or leftover - sizeof(Header)
            newHeader->free = true;
            newHeader->next = current->next;
            newHeader->prev = current;
            
            current->free = false;
            current->size = requestedSize;

            if (current->next != nullptr)
            {
                current->next->prev = newHeader;
            }

            current->next = newHeader;
        }

        // Check next and prev blocks: if they are free, merge with them.
        // Prevents external fragmentation to some extent
        void coalesce(Header* current)
        {
            // Merge with the next block
            if (current->next != nullptr && current->next->free)
            {
                Header* nextBlock = current->next;
                current->size = nextBlock->size + HEADER_SIZE + current->size;
                current->free = true;

                current->next = nextBlock->next;

                if (current->next != nullptr)
                {
                    current->next->prev = current;
                }

            }

            // Merge with the prev block
            if (current->prev != nullptr && current->prev->free)
            {
                Header* prevBlock = current->prev;
                prevBlock->size = prevBlock->size + HEADER_SIZE + current->size;
                prevBlock->free = true;

                prevBlock->next = current->next;

                if (prevBlock->next != nullptr)
                {
                    prevBlock->next->prev = prevBlock;
                }
            }
        }
};