#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_set>

using namespace std;

// source: "0000" target: "0202"
//
// deadends = ["0201", "0101", "0102", "1212", "2002"]
// O/P: 6

vector<string> getNeighbours(const string s)
{
    vector<string> neighbours;

    for(int i=0; i<4; i++)
    {
        string temp = s;
        temp[i] = (((s[i] - '0') + 1) % 10) + '0';
        neighbours.push_back(temp);

        temp[i] = (((s[i] - '0') - 1 + 10) % 10) + '0'; // IMP
        neighbours.push_back(temp);
    }

    return neighbours;
}


int openLock(vector<string>& deadends, string target)
{
    string src = "0000";
    unordered_set<string> deadSet(deadends.begin(), deadends.end());

    if (deadSet.contains(src) || deadSet.contains(target))
    {
        return -1;
    }

    queue<pair<int, string>> q; // {dist, node}
    q.push({0, src});
    unordered_set<string> visited;
    visited.insert(src);

    while (!q.empty())
    {
        string node = q.front().second;
        int d = q.front().first;
        q.pop();

        if (node == target)
        {
            return d;
        }

        for (auto it: getNeighbours(node))
        {
            if (!visited.contains(it) && !deadSet.contains(it))
            {
                visited.insert(it);
                q.push({d+1, it});
            }
        }
    }
    return -1;
    
}

int main()
{
    vector<string> deadends {"0201", "0101", "0102", "1212", "2002"};
    string target = "0202";

    std::cout << openLock(deadends, target) << '\n';

    int a = (-1 + 10) % 10;
    std::cout << a << '\n';

    return 0;
}