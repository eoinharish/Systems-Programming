#include <iostream>
using namespace std;

class Buffer
{
    int* data;

    public:
        Buffer(){
            data = new int[10];
        }

        ~Buffer(){
            delete[] data;
        }
};

int main()
{
    Buffer b1;
    Buffer b2(b1);

    return 0;
}

// What happens

// Buffer b2(b1); invokes the implicitly generated copy constructor, which does a shallow copy: 
// it copies the data pointer value, not the array elements. Now both objects point to the same heap block:

// b1.data ──┐
//           ├──> [ int[10] ]
// b2.data ──┘

// When main returns, b2 is destroyed first and calls delete[] data, freeing the array. 
// Then b1 is destroyed and calls delete[] data on the same, already-freed pointer. 
// That is UNDEFINED BEHAVIOUR: typically a crash (“double free or corruption”), or silent heap corruption.

// Output of the program:
// free(): double free detected in tcache 2
// Aborted                    (core dumped) ./a.out

// Since we are managing a resource manually (implemented a destructor), we cannot rely on default copy ctor/assignment operator 
// and hence we need to implement our own copy ctor/ assignment operator
// The fix: Rule of Three / Five

// Since the class owns a resource, it needs a proper copy constructor and copy assignment operator (and ideally move operations):
