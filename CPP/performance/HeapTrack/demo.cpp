#include <iostream>
#include <vector>
#include <string>

// Run strace to check if allocate triggered syscall or not
// cmd: strace -e trace=mmap,brk,munmap ./demo

void allocate()
{
    int sum = 0;
    std::vector<char> buffer(2000000); // > 10^6 bytes => system call mmap()
    for(int i=0; i < 10; i++)
    {
        buffer[i] = i;
    }
}

int main()
{
    std::cout << "Ready. Press Enter to allocate...\n";
    std::cin.get();

    allocate();

    std::cout << "Allocated\n";
    std::cin.get();

    return 0;
}