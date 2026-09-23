

// -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------
// SERIALIZATION
string serialize(TreeNode* root)
{
    if (root == nullptr){
        return "";
    }

    string s;
    dfs(root, s);
    return s;
}

// preorder
void dfs(TreeNode* root, string& s)
{
    // Base condition
    if(root == nullptr){
        s += ",N";
        return;
    }

    if(s.length() != 0){
        s += ",";
    }

    s += to_string(root->val);

    dfs(root->left, s);
    dfs(root->right, s);

}
// -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------
// DE-SERIALIZATION
TreeNode* deserialize(string s)
{
    if (s.length() == 0){
        return nullptr;
    }
    int i=0;
    return dfs(i, s);
}

TreeNode* dfs(int& i, string& s)
{
    size_t pos = s.find(',', i);
    string token = s.substr(i, pos-i);

    // Base condition
    // IMP: remember to advance i
    if (token == "N")
    {
        i = pos + 1;
        return nullptr;
    }

    TreeNode* root = new TreeNode(stoi(token));
    i = pos+1;

    root->left = dfs(i, s);
    root->right = dfs(i, s);

    return root;
}

int main()
{

}