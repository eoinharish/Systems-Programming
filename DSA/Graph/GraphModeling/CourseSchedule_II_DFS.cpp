
// There are a total of numCourses courses you have to take, labeled from 0 to numCourses - 1.
// You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must take course
// bi first if you want to take course ai.

// For example, the pair [0, 1], indicates that to take course 0 you have to first take course 1.
// Return the ordering of courses you should take to finish all courses. If there are many valid answers, return any of them.
// If it is impossible to finish all courses, return an empty array.


class Solution {
public:

    // DFS + Detect cycle + Stack(for linear ordering)
    bool dfs(int node, vector<int>& vis, vector<int>& pathVis, vector<vector<int>>& adj, stack<int>& st)
    {
        vis[node] = 1;
        pathVis[node] = 1;

        for(auto it: adj[node])
        {
            if (!vis[it])
            {
                if (dfs(it, vis, pathVis, adj, st) == true)
                {
                    return true;
                }
            }
            else if (pathVis[it] == 1)
            {
                return true;
            }
        }

        st.push(node); // In DFS topological sorting, push a node only after all its outgoing neighbors have been processed.
        pathVis[node] = 0;
        return false;
    }

    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites)
    {
        vector<vector<int>> adj(numCourses);

        for(auto it: prerequisites)
        {
            adj[it[1]].push_back(it[0]);
        }

        vector<int> vis(numCourses, 0);
        vector<int> pathVis(numCourses, 0);
        stack<int> st;
        vector<int> ans;
        
        for (int i=0; i < numCourses; i++)
        {
            if (!vis[i])
            {
                if (dfs(i, vis, pathVis, adj, st) == true)
                {
                    return ans; // empty vector
                }
            }
        }

        while (!st.empty())
        {
            ans.push_back(st.top());
            st.pop();
        }

        return ans;
    }
};

// You can also use simple vector for stack and pass it to the dfs call.
// At the end, reversed vector will be your answer.