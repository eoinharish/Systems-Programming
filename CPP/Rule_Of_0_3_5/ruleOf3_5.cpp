#include <iostream>
using namespace std;

class Buffer
{
    public:
        int* data;

    public:
        Buffer()
        {
            cout << "Default ctor called\n";
            data = new int[10];
            for (int i=0; i<10; i++)
            {
                data[i] = i;
            }
        }

        Buffer(const Buffer& other)
        {
            cout << "Copy ctor called\n";
            data = new int[10];
            for(int i=0; i<10; i++)
            {
                data[i] = other.data[i];
            }
        }

        Buffer& operator=(const Buffer& other)
        {
            cout << "Copy assign called\n"; // b2 = b3;
            if (this != &other)
            {
                delete[] data;

                data = new int[10];
                for(int i=0; i<10; i++)
                {
                    data[i] = other.data[i];
                }
            }   
            return *this;

        }

        Buffer(Buffer&& other)
        {
            cout << "Move ctor called\n"; 
            // Buffer b2 = std::move(b1);
            data = other.data;
            other.data = nullptr;
            delete other.data;
        }

        Buffer& operator=(Buffer&& other)
        {
            cout << "Move assign called\n"; 
            if (this != &other)
            {
                // b2 = std::move(b1);
                delete[] data;
                data = other.data;
                other.data = nullptr;
                delete other.data;
            }
            return *this;
        }

        ~Buffer(){
            delete[] data;
        }
};

int main()
{
    Buffer b1;
    Buffer b2(b1); // copy ctor

    for(int i=0; i<10; i++)
    {
        cout << b2.data[i] << " ";
    }
    cout << endl;

    Buffer b3;
    b3 = b2; // copy assign

    Buffer b4 = std::move(b3);
    b4 = std::move(b2);

    return 0;
}

// The fix: Rule of Three / Five

// Since the class owns a resource, it needs a proper copy constructor and copy assignment operator (and ideally move operations):
