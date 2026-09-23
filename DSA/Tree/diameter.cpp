
int dfs(TreeNode* root, int& res)
{
    if (!root) return 0;

    int left = dfs(root->left, res);
    int right = dfs(root->right, res);

    res = max(res, left + right); // if diameter passes through that node, update result

    return 1 + max(left, right); // function will return height (if diameter does not pass through that node)
}

int diameterOfBinaryTree(TreeNode* root)
{
    if (!root) return 0;

    int res = 0;
    dfs(root, res);
    return res;
}

int main()
{

}