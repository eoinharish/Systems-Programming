// Path Sum 1

// Given the root of a binary tree and an integer targetSum, 
// return true if the tree has a root-to-leaf path such that adding up all the values along the path equals targetSum.

// A leaf is a node with no children.

bool hasPathSum(TreeNode* root, int targetSum)
{
    if(!root){
        return false;
    }
    
    int sum = 0;
    return dfs(root, sum, targetSum);
}

// Solution 1: Without sum as reference
// no need to reduce the sum when backtrack, as sum is not passed by ref
bool dfs(TreeNode* root, int sum, int target)
{
    if(root == nullptr){
        return false;
    }

    sum += root->val;
    // check if it's leaf
    if (root->left == nullptr && root->right == nullptr)
    {
        if (sum == target)
        {
            return true;
        }
        return false;
    }

    return dfs(root->left, sum, target) || dfs(root->right, sum, target);
}

// Solution 2: With sum as reference
// Need to reduce the sum when backtrack, as sum is passed by ref
bool dfs(TreeNode* root, int& sum, int target)
{
    if(root == nullptr){
        return false;
    }

    sum += root->val;
    // check if it's leaf node
    if (root->left == nullptr && root->right == nullptr)
    {
        if (sum == target)
        {
            return true;
        }
        sum -= root->val;
        return false;
    }

    if (dfs(root->left, sum, target) == true) return true;
    if (dfs(root->right, sum, target) == true) return true;

    sum -= root->val;

    return false;
}
