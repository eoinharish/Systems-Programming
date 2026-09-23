#include <vector>
using namespace std;

class Solution {
public:

    int timer = 1;

    void dfs(int node, int parent, vector<int>& vis, vector<int> adj[], vector<int>& tin,
            vector<int>& low, vector<vector<int>>& bridges)
    {
        vis[node] = 1;
        tin[node] = low[node] = timer;
        timer++;

        for(auto it: adj[node])
        {
            if (it == parent) continue;

            if (!vis[it])
            {
                dfs(it, node, vis, adj, tin, low, bridges);
                low[node] = min(low[node], low[it]);
                if(low[it] > tin[node])
                {
                    bridges.push_back({it, node});
                }
            }
            else
            {
                low[node] = min(low[node], low[it]);
            }
        }
    }

    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections)
    {
        vector<int> adj[n];

        for(const auto& connection: connections)
        {
            adj[connection[0]].push_back(connection[1]);
            adj[connection[1]].push_back(connection[0]);
        }

        // Configuration for DFS (vis[], parent) + Tarjan (tin[], low[])
        vector<int> vis(n, 0);
        vector<int> tin(n, 0); // time of insertion
        vector<int> low(n, 0); // lowest time it can be reached
        vector<vector<int>> bridges; // output

        dfs(0, -1, vis, adj, tin, low, bridges);
        return bridges;
    }
};

//                dfs (0) tin=1, low=1
//                       /                   
//             dfs (1) tin=2, low=2 (updated low=1 after dfs(2))
//             /                             /
// dfs (2) tin=3, low=1                     dfs (3) tin=4, low=4
//
//(2's low updated to 1;  pick low from its adjacent node 0
//(can pick since 0 is not its parent))


//   dfs(node)
//   dfs(it)  can be a bridge if low[it] > tin[node]

int main()
{
    
}