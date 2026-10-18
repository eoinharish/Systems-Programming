class Solution {
public:

    vector<vector<int>> threeSum(vector<int>& nums)
    {
        // Sort the input array
        sort(nums.begin(), nums.end());

        int n = nums.size();
        vector<vector<int>> ans;

        // Fix i
        for(int i = 0; i <= n-3; i++)
        {
            // Skip duplicate i's
            if (i > 0 && nums[i] == nums[i-1])
            {
                continue;
            }

            int sum = nums[i];

            // Set L, R
            int L = i+1;
            int R = n-1;
            int target = 0 - nums[i];

            while(L < R)
            {
                if (nums[L] + nums[R] == target)
                {
                    ans.push_back({nums[i], nums[L], nums[R]});
                    
                    L++;
                    R--;

                    // Skip duplicate L's
                    while (L < R && nums[L] == nums[L-1])
                    {
                        L++;
                    }
                    // Skip duplicate R's
                    while(L < R && nums[R] == nums[R+1])
                    {
                        R--;
                    }
                }
                else if (nums[L] + nums[R] < target)
                {
                    L++;
                }
                else
                {
                    R--;
                }
            }
        }
        
        return ans;
        
    }
};