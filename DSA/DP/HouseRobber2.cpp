#include <vector>

class Solution {
public:

    // Recursion + Memo
    // int f(int ind, vector<int>& nums, vector<int>& dp)
    // {
    //     if (ind < 0)
    //     {
    //         return 0;
    //     }

    //     if (ind == 0)
    //     {
    //         return nums[ind];
    //     }

    //     if (dp[ind] != -1)
    //     {
    //         return dp[ind];
    //     }

    //     int pick = nums[ind] + f(ind-2, nums, dp);
    //     int notPick = f(ind-1, nums, dp);

    //     return dp[ind] = max(pick, notPick);
    // }

    // space optimized
    int f(vector<int>& nums)
    {
        int n = nums.size();
        int prev = nums[0], prev2 = 0;

        for(int ind=1; ind<n; ind++) // loop from 1 to n-1
        {
            int pick = nums[ind];
            if (ind > 1)
            {
                pick += prev2; // pick += dp[ind-2]
            }

            int notPick = prev; // notPick = dp[ind-1]

            int curi = max(pick, notPick); // dp[ind] = max(pick, notPick);

            prev2 = prev;
            prev = curi;
        }
        
        return prev; // dp[ind-1]
    }

    int rob(vector<int>& nums)
    {
        int n = nums.size();
        
        // V.IMP
        if (n == 1){
            return nums[0];
        }

        vector<int> temp1(nums.begin(), nums.end()-1);
        vector<int> temp2(nums.begin()+1, nums.end());

        // vector<int> dp1(n-1, -1);
        // vector<int> dp2(n-1, -1);

        // int ans1 = f(n-2, temp1, dp1);
        // int ans2 = f(n-2, temp2, dp2);
        //
        //        (OR)
        // use space optimized wrapper
        int ans1 = f(temp1);
        int ans2 = f(temp2);

        return max(ans1, ans2);
        
    }
};
int main()
{

}