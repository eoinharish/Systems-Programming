#include <vector>

using namespace std;

class Solution {
public:

    void f(int ind, vector<int>& ds, vector<vector<int>>& res, vector<int>& nums, int target)
    {
        if (target == 0){
            res.push_back(ds);
            return;
        }
        if (ind == nums.size()){
            return;
        }

        // pick
        if (nums[ind] <= target)
        {
            ds.push_back(nums[ind]);
            f(ind, ds, res, nums, target - nums[ind]);
            ds.pop_back();
        }

        // not pick
        f(ind + 1, ds, res, nums, target);
        
    }

    vector<vector<int>> combinationSum(vector<int>& nums, int target)
    {
        vector<int> ds;
        vector<vector<int>> res;

        f(0, ds, res, nums, target);

        return res;
    }
};

int main()
{
    return 0;
}