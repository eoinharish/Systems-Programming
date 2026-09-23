#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:

    void backtrack(int ind, vector<int>& ds, vector<vector<int>>& res, vector<int>& nums, int target)
    {
        if (target == 0)
        {
            res.push_back(ds);
            return;
        }

        if(ind == nums.size())
        {
            return;
        }

        for(int i=ind; i<nums.size(); i++)
        {
            if(i != ind && nums[i] == nums[i-1])
            {
                continue; // escape duplicate paths
            }

            if(nums[ind] <= target)
            {
                ds.push_back(nums[i]);
                backtrack(i+1, ds, res, nums, target-nums[i]);
                ds.pop_back();
            }
            else
            {
                return;
            }
        }
    }
    vector<vector<int>> combinationSum2(vector<int>& nums, int target)
    {
        sort(nums.begin(), nums.end());

        vector<int> ds;
        vector<vector<int>> res;

        backtrack(0, ds, res, nums, target);
        return res;
    }
};

// f(ind, ds, target)
// f(0, [], 5)      1,2,2,2,5  [1] [2] [5]  
//
// Remeber: Run loop inside recursive call
//
// f(1,[1],4), f(2,[2],3), f(5,[5],0)     1,2,5    (skip the 2's at index 2,3)    
//
// Remember: Run loop inside recursive call:
//
// f(1,[1],4) -> f(2,[1,2],2), f(5,[1,5],-1):don't call    [list size = 2]
// f(2,[2],3) -> f(3,[2,2],1), f(5,[2,5],-2):don't call    [list size = 2]
// f(5,[5],0) -> add into res; return;

// f(3,[2,2],1) -> f(4,[2,2,2],-1):don't call, f(5,[2,2,5],-4):don't call    [list size = 3]
// f(5,[2,5],-2) -> return;

// level 1: [1] [2] [5]
// level 2: [1,2] [1,5] [2,2] [2,5]
// level 3: [1,2,2] [1,2,5] [2,2,2] [2,2,5]
// level 4: [1,2,2,2] [1,2,2,5] [2,2,2,5]
// level 5: [1,2,2,2,5]

int main()
{
    return 0;
}