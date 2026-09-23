#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:

    void dfs(int i, vector<int>& nums, vector<int>& ds, vector<vector<int>>& res)
    {
        if (i == nums.size())
        {
            res.push_back(ds); // [[1 2 3]]
            return;
        }

        ds.push_back(nums[i]); // 1 2 3
        dfs(i+1, nums, ds, res); // dfs(3)
        ds.pop_back(); // 1 2
        dfs(i+1, nums, ds, res); // dfs(3)
    }

    // dfs(2)
    //
    // [1 2 3]
    // dfs(3); // add [1 2 3] into res
    // [1 2]
    // dfs(3); // add [1 2] into res
    //
    // after dfs(2):
    // [[1 2 3] [1 2]]

    // dfs(1)
    // [1 2]
    // dfs(2)
    // [1]
    // dfs(2) // dfs(2): [1 3] dfs(3) [1] dfs(3) -> [1,3] [1]
    //
    // after dfs(1):
    // [[1 2 3] [1 2] [1,3] [1]]

    // dfs(0)
    //
    // 1
    // dfs(1)
    // []
    // dfs(1) // dfs(1): [2] dfs(2) [] dfs(2)
    //        // dfs(2): [2 3] dfs(3) [2] dfs(3) -> [2 3] [2]
    //        // dfs(2): [3] dfs(3) [] dfs(3) -> [3] []
    //
    // after dfs(0):
    // [[1 2 3] [1 2] [1,3] [1] [2 3] [2] [3] []]
    //

    vector<vector<int>> subsets(vector<int>& nums)
    {
        vector<int> ds;
        vector<vector<int>> res;
        dfs(0, nums, ds, res);

        return res;
    }
};

int main()
{
    return 0;
}