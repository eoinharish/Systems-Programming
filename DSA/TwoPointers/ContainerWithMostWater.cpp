class Solution {
public:
    int maxArea(vector<int>& height)
    {
        int n = height.size();
        int L = 0;
        int R = n-1;

        int maxArea = 0;

        while (L < R)
        {
            int localArea = (R-L) * min(height[L], height[R]);
            maxArea = max(maxArea, localArea);

            if (height[L] < height[R])
            {
                L++;
            }
            else
            {
                R--;
            }
        }
        return maxArea;
        
    }
};