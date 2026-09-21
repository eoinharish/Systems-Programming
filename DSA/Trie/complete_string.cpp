#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node
{
    Node* links[26] = {nullptr};
    bool flag = false;
};

class Trie
{
    private:
        Node* root;

    public:
        Trie()
        {
            root = new Node();
        }

        ~Trie()
        {
            delete root;
        }

        void insert(const string& word)
        {
            Node* node = root;

            for (int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    node->links[index] = new Node();
                }

                node = node->links[index];
            }

            // set end flag (complete word ends there)
            node->flag = true;
        }

        bool checkIfAllPrefixesExist(const string& word)
        {
            Node* node = root;

            for (char ch: word)
            {
                int index = ch - 'a';
                node = node->links[index];
                if (node->flag == false)
                {
                    return false;
                }
            }
            return true;
        }
};

string completeString(const vector<string>& v)
{
    Trie trie;

    for (const auto& str: v)
    {
        trie.insert(str);
    }

    string ans = "";
    int longest = 0;

    for (const auto& str: v)
    {
        if (trie.checkIfAllPrefixesExist(str))
        {
            if (str.length() > longest)
            {
                longest = str.length();
                ans = str;
            }
            else if (str.length() == longest && str < ans) // we need lexicographical smaller
            {
                ans = str;
            }
        }
    }
    return ans;
}

int main()
{
    // vector<string> input = {"n", "ni", "nin", "ninj", "ninja", "ninga"};
    vector<string> input = {"a", "ab", "abc", "abcd", "abca"};

    cout << completeString(input) << '\n';

    return 0;
}