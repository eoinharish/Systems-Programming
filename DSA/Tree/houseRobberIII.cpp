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

    // {withRoot, withoutRoot}
    //
    // withRoot -> we cannot include left and right
    //             int withRoot = root->val + withoutRootLeft + withoutRootRight;
    //
    // withoutRoot -> no restriction -> take max from left pair (max1) + max from right pair (max2)
    //             int withoutRoot = max(withRootLeft, withoutRootLeft) +
    //                               max(withRootRight, withoutRootRight);
    //
    pair<int, int> dfs(TreeNode* root)
    {
        if (!root) return {0, 0};

        auto [withRootLeft, withoutRootLeft] = dfs(root->left);
        auto [withRootRight, withoutRootRight] = dfs(root->right);

        int withRoot = root->val + withoutRootLeft + withoutRootRight;

        // maximum from left pair + maximum from right pair
        int withoutRoot = max(withRootLeft, withoutRootLeft) +
                          max(withRootRight, withoutRootRight);
        
        return {withRoot, withoutRoot};
    }

    int rob(TreeNode* root)
    {
        if(!root) return 0;

        auto [ans1, ans2] = dfs(root);
        return max(ans1, ans2);
    }
};

int main()
{

}