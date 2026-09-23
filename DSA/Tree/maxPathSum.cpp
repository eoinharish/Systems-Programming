/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
 
class Solution {
public:

    int dfs(TreeNode* root, int& res)
    {
        if (root == nullptr)
        {
            return 0;
        }

        int left = dfs(root->left, res);
        int right = dfs(root->right, res);

        if (left < 0) left = 0;   // don't take negative
        if (right < 0) right = 0; // don't take negative

        res = max(res, left + right + root->val); // update result

        return max(left, right) + root->val; // IMP
    }

    int maxPathSum(TreeNode* root)
    {
        if (root == nullptr){
            return 0;
        }

        int res = root->val;

        dfs(root, res);
        
        return res;
    }
};

int main()
{

}