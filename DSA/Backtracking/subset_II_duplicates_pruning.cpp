#include <vector>
#include <algorithm>

using namespace std;


// Say out loud: At a particular level, Can I pick this?
// Don't pick the same element again for that node
void backtrack(int ind, vector<int>& nums, vector<int>& ds, vector<vector<int>>& res)
{
    res.push_back(ds); // each node is a subset

    for(int i=ind; i < nums.size(); i++)
    {
        if (i != ind && nums[i] == nums[i-1])
        {
            // At a particular level, never pick the same value twice.
            continue; // avoid the duplicate paths
        }

        ds.push_back(nums[i]); // add
        backtrack(i+1, nums, ds, res); // explore
        ds.pop_back(); // undo
    }
}

// The three-step template

// Almost every backtracking problem fits this:

// Choose: add an element to path.
// Explore: recurse with the updated state (i + 1 because each element is used at most once).
// Undo: pop_back() so the next sibling starts from a clean state.

vector<vector<int>> subsetsWithDup(vector<int>& nums)
{
    sort(nums.begin(), nums.end()); // so that we can skip duplicates

    vector<int> ds;
    vector<vector<int>> res; 

    backtrack(0, nums, ds, res);
    
    return res;
}

int main()
{
    return 0;
}