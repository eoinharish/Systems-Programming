#include <vector>
using namespace std;

// There are several consecutive houses along a street, each of which has some money inside. There is also a robber, who wants to steal money from the homes, but he refuses to steal from adjacent homes.
//
// The capability of the robber is the maximum amount of money he steals from one house of all the houses he robbed.
//
// You are given an integer array nums representing how much money is stashed in each house. More formally, the ith house from the left has nums[i] dollars.
//
// You are also given an integer k, representing the minimum number of houses the robber will steal from. It is always possible to steal at least k houses.
//
// Return the minimum capability of the robber out of all the possible ways to steal at least k houses.
//
//Example 1:

// Input: nums = [2,3,5,9], k = 2
// Output: 5
// Explanation: 
// There are three ways to rob at least 2 houses:
// - Rob the houses at indices 0 and 2. Capability is max(nums[0], nums[2]) = 5.
// - Rob the houses at indices 0 and 3. Capability is max(nums[0], nums[3]) = 9.
// - Rob the houses at indices 1 and 3. Capability is max(nums[1], nums[3]) = 9.
// Therefore, we return min(5, 9, 9) = 5.

bool isValid(int mid, vector<int>& nums, int k)
{
    int houses = 0;

    for(int i=0; i<nums.size(); i++)
    {
        if(nums[i] <= mid)
        {
            houses++;
            i++; // pick krne par total index+2 krna hai (as can't steel from adj houses)
        }
    }

    // kya mein capability = mid ke saath minimum k houses
    // se steal kar pa raha hu
    return houses >= k;
}

int minCapability(vector<int>& nums, int k)
{
    int n = nums.size();

    int low = *min_element(nums.begin(), nums.end()); // 2
    int high = *max_element(nums.begin(), nums.end()); // 9

    int ans = high;

    while (low <= high)
    {
        int mid = low + (high-low)/2;

        if (isValid(mid, nums, k))
        {
            ans = mid;
            high = mid-1;
        }
        else
        {
            low = mid+1;
        }
    }
    return ans;         
}

int main()
{

}