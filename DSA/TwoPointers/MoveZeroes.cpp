

// Crux: Move non-zero elements to the left
// [0...L]     [L+1....R] [R...n-1]
// non-zero     zeroes    unexplored
//
// 1 5 8 0 6 0 0 3 4
// 1 5 8 6 3 4 0 0 0

class Solution {
public:

    void moveZeroes(vector<int>& nums)
    {
        int n = nums.size();
        int L = 0;
        int R = 0;

        while (R < n)
        {
            if (nums[R] != 0)
            {
                swap(nums[L], nums[R]);
                L++;
            }
            R++;
        }
    }
};
