// Path Sum II

// Given the root of a binary tree and an integer targetSum, 
// return all root-to-leaf paths where the sum of the node values in the path equals targetSum. 
// Each path should be returned as a list of the node values, not node references.

// A root-to-leaf path is a path starting from the root and ending at any leaf node. A leaf is a node with no children.

vector<vector<int>> pathSum(TreeNode* root, int targetSum)
{
    vector<vector<int>> res;
    if (root == nullptr){
        return res;
    }

    int sum = 0;
    vector<int> ds;

    dfs(root, sum, targetSum, ds, res);

    return res;
}

// Solution 1: When sum and ds are not passed by ref
// Simple: No need to reduce sum and pop back from ds as they aren't passed by ref
void dfs(TreeNode* root, int sum, int target, vector<int> ds, vector<vector<int>>& res)
{
    if(root == nullptr){
        return;
    }

    sum += root->val;
    ds.push_back(root->val);  

    if (root->left == nullptr && root->right == nullptr)
    {
        if (sum == target)
        {
            res.push_back(ds);
        }
        return;
    }

    dfs(root->left, sum, target, ds, res);
    dfs(root->right, sum, target, ds, res);

}

// Solution 2: When sum and ds are passed by ref
// Properly need to reduce sum and pop back from ds, as they are passed by ref.
void dfs(TreeNode* root, int& sum, int target, vector<int>& ds, vector<vector<int>>& res)
{
    if(root == nullptr){
        return;
    }

    sum += root->val;
    ds.push_back(root->val);  

    if (root->left == nullptr && root->right == nullptr)
    {
        if (sum == target)
        {
            res.push_back(ds);
        }

        sum -= root->val;
        ds.pop_back();
        return;
    }

    dfs(root->left, sum, target, ds, res);
    dfs(root->right, sum, target, ds, res);

    sum -= root->val;
    ds.pop_back();

}
