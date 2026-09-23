class Solution {
  public:

    int timer = 1;

    void dfs(int node, int parent, vector<int>& vis, vector<int> adj[], vector<int>& tin,
            vector<int>& low, unordered_set<int>& ap)
    {
        vis[node] = 1;
        tin[node] = low[node] = timer;
        timer++;
        
        int child = 0;

        for(auto it: adj[node])
        {
            if (it == parent) continue;

            if (!vis[it])
            {
                dfs(it, node, vis, adj, tin, low, ap);
                low[node] = min(low[node], low[it]);

                if (low[it] >= tin[node] && parent != -1)
                {
                    ap.insert(node);
                }
                child++;
            }
            else
            {
                low[node] = min(low[node], tin[it]);
                
            }
        }
        
        if (child > 1 && parent == -1)
        {
            ap.insert(node);
        }

    }
    vector<int> articulationPoints(int V, vector<vector<int>>& edges)
    {
        vector<int> adj[V];
        for(auto it: edges)
        {
            adj[it[0]].push_back(it[1]);
            adj[it[1]].push_back(it[0]);
        }

        vector<int> vis(V,0);
        vector<int> tin(V,0);
        vector<int> low(V,0);

        unordered_set<int> ap;
        
        for(int i=0; i<V; i++)
        {
            if(!vis[i])
            {
                dfs(i, -1, vis, adj, tin, low, ap);
            }
        }
        
        vector<int> ans(ap.begin(), ap.end());
        if(ans.empty()) return {-1};
        return ans;
    }
};