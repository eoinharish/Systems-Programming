#include <vector>
using namespace std;

class Solution {
public:

    int f(int ind, vector<int>& nums, vector<int>& dp)
    {
        if (ind < 0)
        {
            return 0;
        }

        if (ind == 0)
        {
            return nums[ind];
        }

        if (dp[ind] != -1)
        {
            return dp[ind];
        }

        int pick = nums[ind] + f(ind-2, nums, dp);
        int notPick = f(ind-1, nums, dp);

        return dp[ind] = max(pick, notPick);
    }

    int rob(vector<int>& nums)
    {
        int n = nums.size();
        // vector<int> dp(n, -1);
        // return f(n-1, nums, dp); // recursion + memo

        // vector<int> dp(n, 0); // For tabulation
        // dp[0] = nums[0]; // base case

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

        // dp: [1, 2, 4, 4]
        // for (int i=0; i<n; i++)
        // {
        //     cout << dp[i] << " ";
        // }
        
        return prev; // dp[ind-1]
    }
};

int main()
{
    return 0;
}