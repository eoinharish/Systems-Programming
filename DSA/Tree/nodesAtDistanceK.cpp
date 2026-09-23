#include <bits/stdc++.h>
using namespace std;

void markParents(TreeNode* root, unordered_map<TreeNode*, TreeNode*>& parent)
{
    if (root == nullptr) return;

    queue<TreeNode*> q;
    q.push(root);

    while(!q.empty())
    {
        TreeNode* node = q.front();
        q.pop();

        if(node->left){
            q.push(node->left);
            parent[node->left] = node;
        }
        if(node->right){
            q.push(node->right);
            parent[node->right] = node;
        }
    }
}

vector<int> distanceK(TreeNode* root, TreeNode* target, int k)
{
    vector<int> ans;
    if (k==0)
    {
        ans.push_back(target->val);
        return ans;
    }
    
    unordered_map<TreeNode*, TreeNode*> parent; // node -> parent
    markParents(root, parent);

    queue<TreeNode*> q;
    q.push(target);
    unordered_set<TreeNode*> vis; // To avoid pushing the nodes again into the queue
    vis.insert(target);

    int level = 0;
    while(!q.empty())
    {
        int size = q.size();

        for(int i=0; i<size; i++)
        {
            TreeNode* node = q.front();
            q.pop();

            if(node->left && !vis.contains(node->left)){
                vis.insert(node->left);
                q.push(node->left);
            }
            if(node->right && !vis.contains(node->right)){
                vis.insert(node->right);
                q.push(node->right);
            }
            if(parent[node] && !vis.contains(parent[node])){
                vis.insert(parent[node]);
                q.push(parent[node]);
            }
        }
        level++;
        
        if (level == k)
        {
            while(!q.empty())
            {
                ans.push_back(q.front()->val);
                q.pop();
            }
        }
    }
    return ans;
    
}