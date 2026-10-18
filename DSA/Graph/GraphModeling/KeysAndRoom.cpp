// There are n rooms labeled from 0 to n - 1 and all the rooms are locked except for room 0. 
// Your goal is to visit all the rooms. However, you cannot enter a locked room without having its key.

// When you visit a room, you may find a set of distinct keys in it. Each key has a number on it, denoting which room it unlocks, 
// and you can take all of them with you to unlock the other rooms.

// Given an array rooms where rooms[i] is the set of keys that you can obtain if you visited room i, 
// return true if you can visit all the rooms, or false otherwise.

// Example 2:

// Input: rooms = [[1,3],[3,0,1],[2],[0]]
// Output: false
// Explanation: We can not enter room number 2 since the only key that unlocks it is in that room.

class Solution {
public:

    bool canVisitAllRooms(vector<vector<int>>& rooms)
    {
        int n = rooms.size();
        vector<int> vis(n, 0);

        dfs(0, vis, rooms); // src node = 0

        for (auto it: vis)
        {
            // check if any room is unvisited
            if (it == 0)
            {
                return false;
            }
        }

        return true;
    }

    void dfs(int node, vector<int>& vis, vector<vector<int>>& rooms)
    {
        vis[node] = 1;

        for(auto it: rooms[node])
        {
            if (!vis[it])
            {
                dfs(it, vis, rooms);
            }
        }
    }
};

// 0 -> [1]
// 1 -> [2]
// 2 -> [3]
// 3 -> []

// 0 -> [1,3]
// 1 -> [3,0,1]
// 2 -> [2]
// 3 -> [0]