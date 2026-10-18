#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n = 4;
    vector<int> a(n, 1);
    vector<int> b(n, 2);

    vector<int> c(n, 0);

    for(int i=0; i<n; i++)
    {
        c[i] = a[i] + b[i];
    }

    for(auto it: c)
    {
        cout << it << " ";
    }
    cout << endl;
}