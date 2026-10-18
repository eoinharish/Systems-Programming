

class Solution {
public:

    bool bfs(int src, vector<int>& color, vector<vector<int>>& graph)
    {
        queue<int> q;
        q.push(src);
        color[src] = 0;

        while(!q.empty())
        {
            int node = q.front();
            q.pop();

            for(auto it: graph[node])
            {
                // if neighbour is not colored, color it with the opposite color
                if (color[it] == -1)
                {
                    color[it] = !color[node];
                    q.push(it);
                }
                else if(color[it] == color[node])
                {
                    // color of neighbour and its parent is the same
                    // hence adj nodes are not colored differently
                    return false;
                }
            }
        }
        return true;
    }
    
    bool isBipartite(vector<vector<int>>& graph)
    {
        int n = graph.size();

        vector<int> color(n, -1);

        // IMP: component stuff 
        for(int i=0; i<n; i++)
        {
            if (color[i] == -1)
            {
                if (bfs(i, color, graph) == false)
                {
                    return false;
                }
            }
        }

        return true;
    }
};