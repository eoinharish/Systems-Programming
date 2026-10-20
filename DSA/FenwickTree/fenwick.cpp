#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class FenwickTree
{
    public:
        int N;
        vector<int> fen;

    public:

        FenwickTree(int n = 8): N(n), fen(n, 0)
        {}

        void update(int i, int val)
        {
            while (i < N)
            {
                fen[i] += val;
                i += (i & -i);
            }
        }

        // 1..i
        int sum(int i)
        {
            int s = 0;
            while (i > 0)
            {
                s += fen[i];
                i -= (i & -i);
            }
            return s;
        }

        int rangeSum(int L, int R)
        {
            return sum(R) - sum(L-1);
        }

        int findLogFloor(int n)
        {
            // 1,  2,   4,    8,    16,   32
            // 1 , 10, 100, 1000 10000, 100000
            int count = 0;
            n = n >> 1; // divide by 2
            while(n > 0)
            {
                count++;
                n = n >> 1; // divide by 2
            }
            return count;
        }

        int search(int val)
        {
            // int LOGN = findLogFloor(N); // 3
            
            int L = 0;
            int prevSum = 0;

            for(int i = log2(N); i>=0; i--)
            {
                int index = L + (1 << i);
                if (fen[index] + prevSum < val)
                {
                    L = index;
                    prevSum += fen[index];
                }
            }
            return L+1;
        }
};

int main()
{
    int N = 10;
    vector<int> input(N+1);
    
    for(int i=1; i < N+1; i++)
    {
        input[i] = i * 10;
    }

    FenwickTree tree(N+1);
    for(int i = 1; i < N+1; i++)
    {
        tree.update(i, input[i]);
    }

    for(int i=0; i < N+1; i++)
    {
        cout << input[i] << " ";
    }
    cout << "\n";

    for(int i=0; i < N+1; i++)
    {
        cout << tree.fen[i] << " ";
    }
    cout << "\n";

    cout << tree.sum(6) << endl;
    cout << tree.rangeSum(3, 6) << endl;
    cout << tree.search(210) << endl;


    return 0;
}