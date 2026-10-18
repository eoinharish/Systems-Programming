
// Input: nums = [2,0,2,1,1,0]

// Output: [0,0,1,1,2,2]

class Solution {
public:
    void sortColors(vector<int>& nums)
    {
        int n = nums.size();
        int L = 0;
        int R = n-1;

        // L     R
        // i to scan the array
        int i = 0;

        while (i <= R)
        {
            if (nums[i] == 0)
            {
                swap(nums[i], nums[L]);
                L++;
                i++;
            }
            else if (nums[i] == 2)
            {
                swap(nums[i], nums[R]);
                R--;
                // don't increment i immediately as nums[i] now can be 2
            }
            else
            {
                i++; // continue
            }
        }
    }
};