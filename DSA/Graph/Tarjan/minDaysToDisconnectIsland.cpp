class Solution {
public:

    vector<vector<int>> directions {{-1, 0}, {0,+1}, {+1,0}, {0,-1}};
    int n,m;

    int timer = 1;
    bool flag = false; // check if articulation point exists

    void dfs(int row, int col, vector<vector<int>>& parent, vector<vector<int>>& vis, vector<vector<int>>& grid,
            vector<vector<int>>& tin, vector<vector<int>>& low, int* area)
    {
        vis[row][col] = 1;
        tin[row][col] = timer;
        low[row][col] = timer;
        timer++;
        *area += 1;
        int child = 0;

        for (auto dir: directions)
        {
            int nrow = row + dir[0];
            int ncol = col + dir[1];

            if (nrow < 0 || nrow >= n || ncol < 0 || ncol >= m)
                continue;

            if (parent[row][col] == (nrow * m + ncol)) continue;
            
            if (grid[nrow][ncol] == 1 && !vis[nrow][ncol])
            {
                parent[nrow][ncol] = row * m + col;
                dfs(nrow, ncol, parent, vis, grid, tin, low, area);

                low[row][col] = min(low[row][col], low[nrow][ncol]);

                if (low[nrow][ncol] >= tin[row][col] && parent[row][col] != -1)
                {
                    flag = true;
                }

                child++;
            }
            else if (grid[nrow][ncol] == 1 && vis[nrow][ncol])
            {
                low[row][col] = min(low[row][col], tin[nrow][ncol]);
            }
        }

        if (child > 1 && parent[row][col] == -1)
        {
            flag = true;
        }
    }

    int minDays(vector<vector<int>>& grid)
    {
        n = grid.size();
        m = grid[0].size();

        vector<vector<int>> vis(n, vector<int>(m, 0));
        
        vector<vector<int>> tin(n, vector<int>(m, 0));
        vector<vector<int>> low(n, vector<int>(m, 0));
        vector<vector<int>> parent(n, vector<int>(m, -1)); // IMP

        int cnt = 0;
        int area = 0;
        for (int i=0; i<n; i++)
        {
            for(int j=0; j<m; j++)
            {
                if (grid[i][j] == 1 && !vis[i][j])
                {
                    dfs(i, j, parent, vis, grid, tin, low, &area);
                    cnt++;
                }
            }
        }

        if (cnt != 1){
            return 0;
        }
        if (area == 1){ // IMP edge case for only single 1 in the entire grid
            return 1;
        }

        if (flag == true){
            return 1;
        }
        return 2;
    }
};

// 1. Count the no of islands
// if # of islands != 1 return 0 (already disconnected)

// 2. Check for area if island
// if area of island == 1, return 1 day
//
// 3. Check if articulation point exists
// If articulation point exist, return 1

// return 2 otherwise

int main()
{
    
}