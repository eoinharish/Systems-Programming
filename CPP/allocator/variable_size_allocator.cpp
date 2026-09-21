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
};

class FreeListAllocator
{
    private:
        char* arena;
        Header* head; // first block in the whole arena

    public:
        FreeListAllocator(size_t totalSize)
        {
            arena = new char[totalSize];

            // the whole arena starts as ONE big free block
            head = reinterpret_cast<Header*>(arena);
            head->size = totalSize - sizeof(Header);
            head->free = true;
            head->next = nullptr;
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
                    
                    // Ex: block start address: 1000, return address: 1024
                    // As need to reserve 24 bytes for Header
                    return reinterpret_cast<char*>(current) + sizeof(Header);
                }
                current = current->next;
            }

            return nullptr; // no block big enough
        }

        void deallocate(void* ptr)
        {
            if (!ptr) return;

            Header* header = reinterpret_cast<Header*>(reinterpret_cast<char*>(ptr) - sizeof(Header));
            header->free = true;

            coalesce();
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

                if (leftover <= sizeof(Header))
                {
                    return; // not worth splitting, just hand out the whole block
                }

                char* newBlockAddr = reinterpret_cast<char*>(current) + sizeof(Header) + requestedSize;
                Header* newHeader = reinterpret_cast<Header*>(newBlockAddr);
                newHeader->size = current->size - (sizeof(Header) + requestedSize); // or leftover - sizeof(Header)
                newHeader->free = true;
                newHeader->next = current->next;
                
                current->free = false;
                current->size = requestedSize;
                current->next = newHeader;

            }

            // Walk the list; whenever two ADJACENT blocks are both free, merge them.
            // Prevents external fragmentation to some extent
            void coalesce()
            {
                Header* current = head;

                while (current != nullptr && current->next != nullptr)
                {
                    if (current->free && current->next->free)
                    {
                        current->size = current->size + current->next->size + sizeof(Header);
                        current->next = current->next->next;
                        // don't advance - keep checking in case there's a THIRD
                        // free block after this merged one
                    }
                    else
                    {
                        current = current->next;
                    }
                }
            }

};

int main()
{
    FreeListAllocator alloc(200); // small arena, 200B total

    std::cout << "Initial state:\n";
    alloc.debugPrint();

    void* a = alloc.allocate(20);
    std::cout << "mem addr a: " << a <<'\n';
    std::cout << "After allocating 20 bytes (a):\n";
    alloc.debugPrint();
 
    void* b = alloc.allocate(30);
    std::cout << "mem addr b: " << b <<'\n';
    std::cout << "After allocating 30 bytes (b):\n";
    alloc.debugPrint();

    alloc.deallocate(a);
    std::cout << "After freeing a:\n";
    alloc.debugPrint();

    void* c = alloc.allocate(10);
    std::cout << "After allocating 10 bytes (c) - should reuse a's space:\n";
    alloc.debugPrint();

    alloc.deallocate(b);
    alloc.deallocate(c);
    std::cout << "After freeing b and c - should coalesce back together:\n";
    alloc.debugPrint();

    return 0;
}