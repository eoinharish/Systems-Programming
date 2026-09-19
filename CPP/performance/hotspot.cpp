#include <iostream>
#include <vector>

volatile long long result = 0;

void expensive_function()
{
    for (long long i = 0; i < 500'000'000; ++i)
    {
        result += i % 7;
    }
}

void cheap_function()
{
    for (int i = 0; i < 1000; ++i)
    {
        result += i;
    }
}

int main()
{
    cheap_function();
    expensive_function();

    std::cout << "Result: " << result << '\n';
    return 0;
}