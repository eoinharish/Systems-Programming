class Solution {
public:
    int trap(vector<int>& height)
    {
        int n = height.size();

        vector<int> maxL(n, 0); // prefixMax
        vector<int> maxR(n, 0); // suffixMax

        maxL[0] = height[0];
        for(int i = 1; i < n; i++)
        {
            maxL[i] = max (maxL[i-1], height[i]);
        }

        maxR[n-1] = height[n-1];
        for(int i = n-2; i >= 0; i--)
        {
            maxR[i] = max (maxR[i+1], height[i]);
        }

        vector<int> water(n, 0);
        for (int i=0; i<n; i++)
        {
            if (maxL[i] < maxR[i])
            {
                water[i] = max (0, maxL[i] - height[i]);
            }
            else
            {
                water[i] = max (0, maxR[i] - height[i]);
            }
        }

        int totalWaterTrapped = 0;
        for(int i=0; i<n; i++)
        {
            totalWaterTrapped += water[i];
        }
        return totalWaterTrapped;
        
    }
};

// Most optimal O(1) space (2 pointers solution)
// We don't actually need prefix/suffixMax arrays
// We know that shorter of L or R height is the bottleneck
// for any building

class Solution {
public:
    int trap(vector<int>& height)
    {
        int n = height.size();

        int L = 0;
        int R = n-1;

        int maxL = 0;
        int maxR = 0;
        int totalWater = 0;

        while (L < R)
        {
            if (height[L] < height[R])
            {
                // L height is the bottleneck
                maxL = max(maxL, height[L]); // keep a track of maxL so far
                totalWater += max(0, maxL - height[L]);
                L++;

            }
            else
            {
                // R height is the bottleneck
                maxR = max(maxR, height[R]); // keep a track of maxR so far
                totalWater += max(0, maxR - height[R]);
                R--;
            }
        }
        return totalWater;
        
    }
};