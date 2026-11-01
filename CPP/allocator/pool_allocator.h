#pragma once
#include <iostream>
#include <new>

// Fixed-Size Pool Allocator
//
// 1. Pre-allocate one large arena. arena = slotSize * slotCount
//    Take the address stored in arena and treat it as the address of a Node
//    freeList = reinterpret_cast<Node*>(arena)
// 2. Divide arena into fixed-size slots
// 3. Free list = linked list of currently free slots
// 4. "Intrusive free list trick": The free slot itself stores the next pointer. No extra memory for the list nodes
// 5. freeList stores the address of the first free slot
// 6. Allocation is O(1) -> hand out the first free slot (pointed by freeList), move head ahead
// 7. Deallocation is O(1) -> put the slot back at the front of the list

// reinterpret_cast is used when we need to reinterpret the same memory/address as a different, unrelated pointer type.
// In custom allocators, it's commonly used to interpret raw storage as allocator metadata such as a free-list node.

class PoolAllocator
{
    private:

        char* arena; // raw memory
        size_t slotSize; // size of each slot

        struct Node
        {
            Node* next;
        };

        Node* freeList; // head of the free list (points to the first free slot)

    public:

        PoolAllocator(size_t slotSize, size_t slotCount) : slotSize(slotSize)
        {
            // Ex. slotSize = 16B, slotCount = 3, arena starting at 1000
            arena = new char[slotSize * slotCount]; // arena points to address 1000 (1000 - 1047)

            // This line says: "Treat the memory at address 1000 as a Node address, and that's where our free list starts.
            freeList = reinterpret_cast<Node*>(arena); // head of the freeList 1000

            // Walk through every slot and point it at the next one
            // The "next pointer" for slot 0 physically lives inside slot 0's own bytes.
            Node* current = freeList;

            for(size_t i=1; i < slotCount; i++)
            {
                current->next = reinterpret_cast<Node*>(arena + i * slotSize); // 1000 + 16
                current = current->next; // 1016
            }

            current->next = nullptr;

            // Once you allocate() slot 0 and hand it to the user, the user overwrites those bytes with their own data
            // — the next pointer is gone, but that's fine, because slot 0 is no longer part of the free list anyway.
        }

        ~PoolAllocator()
        {
            delete[] arena;
        }

        void* allocate()
        {
            if (freeList == nullptr)
            {
                // throw std::bad_alloc{};
                return nullptr; // pool is full, can't allocate
            }

            Node* slot = freeList; // take the first free slot
            freeList = freeList->next; // move head to the next one
            return slot;
        }

        void deallocate(void* ptr)
        {
            Node* slot = reinterpret_cast<Node*>(ptr);
            slot->next = freeList; // This slot now point to the old head
            freeList = slot; // This slot becomes the new head
        }

};

// struct Order
// {
//     int id; // 4
//     double price; // 8
// }; // 16 bytes (as 4 bytes padding: padding by largest member type)


// A* a = new A(); // normal new operator. Get memory and construct object in that memory.

// A* a = new (mem) A(); // Don't allocate memory. I already have memory at "mem" addr. Construct obj there.
