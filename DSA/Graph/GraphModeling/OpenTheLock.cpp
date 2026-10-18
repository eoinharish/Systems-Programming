// source: "0000" target: "0202"
//
// deadends = ["0201", "0101", "0102", "1212", "2002"]
// O/P: 6

class Solution {
public:

    // "0000"
    vector<string> getNeighbours(string lock)
    {
        vector<string> neighbours; // 8 neighbours

        for(int i=0; i < lock.length(); i++) // i < 4
        {
            char original = lock[i];

            int up = (lock[i] - '0' + 1) % 10;
            lock[i] = up + '0';
            neighbours.push_back(lock);

            lock[i] = original;

            int down = (lock[i] - '0' - 1 + 10) % 10;
            lock[i] = down + '0';
            neighbours.push_back(lock);

            lock[i] = original;
        }

        return neighbours;
    }

    int openLock(vector<string>& deadends, string target)
    {
        string src = "0000";

        queue<pair<string, int>> q; // {node, d}
        q.push({src, 0});

        unordered_set<string> deadSet(deadends.begin(), deadends.end());
        if (deadSet.contains(target) || deadSet.contains(src))
        {
            return -1;
        }

        unordered_set<string> visited;
        visited.insert(src);

        while (!q.empty())
        {
            string node = q.front().first;
            int d = q.front().second;
            q.pop();

            if (node == target)
            {
                return d;
            }

            for(auto it: getNeighbours(node))
            {
                if (!visited.contains(it) && !deadSet.contains(it))
                {
                    q.push({it, d+1});
                    visited.insert(it);
                }
            }
        }
        return -1; 
    }
};