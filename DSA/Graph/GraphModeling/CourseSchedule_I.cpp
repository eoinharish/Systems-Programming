

// There are a total of numCourses courses you have to take, labeled from 0 to numCourses - 1. 
// You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must take course 
// bi first if you want to take course ai.

// For example, the pair [0, 1], indicates that to take course 0 you have to first take course 1.
// Return true if you can finish all courses. Otherwise, return false.

class Solution {
public:

    // Detect cycle
    bool dfs(int node, vector<int>& vis, vector<int>& pathVis, vector<int> adj[])
    {
        vis[node] = 1;
        pathVis[node] = 1;

        for(auto it: adj[node])
        {
            if (!vis[it])
            {
                if (dfs(it, vis, pathVis, adj) == true)
                {
                    return true;
                }
            }
            else if (pathVis[it] == 1)
            {
                return true;
            }
        }

        pathVis[node] = 0;
        return false;
    }

    bool canFinish(int numCourses, vector<vector<int>>& prerequisites)
    {
        vector<vector<int>> adj(numCourses);

        for(auto it: prerequisites)
        {
            adj[it[1]].push_back(it[0]);
        }

        vector<int> vis(numCourses, 0);
        vector<int> pathVis(numCourses, 0);
        
        // Component stuff
        for (int i=0; i < numCourses; i++)
        {
            if (!vis[i])
            {
                // If contains cycle, impossible
                if (dfs(i, vis, pathVis, adj) == true)
                {
                    return false;
                }
            }
        }

        return true;
    }
};