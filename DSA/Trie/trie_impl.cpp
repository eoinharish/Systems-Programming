#include <iostream>

using namespace std;


// Trie Node
// ---------------------------------------------------------------------------
struct Node
{
    Node* links[26] = {nullptr}; // for lowercase a-z
    bool flag = false; // isEndofWord

    ~Node()
    {
        for(int i=0; i<26; i++)
        {
            delete links[i]; // recursively triggers ~Node() on the child
        }
    }
};

// ---------------------------------------------------------------------------
// INSERT, SEARCH, STARTSWITH : Check if a word or a prefix exists
// ---------------------------------------------------------------------------
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
            delete root; // this alone triggers the whole recursive cleanup
        }

        void insert(string word)
        {
            Node* node = root;

            // O(word length)
            for (int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    node->links[index] = new Node();
                }

                // move to the next node
                node = node->links[index];
            }

            // set end flag (complete word ends there)
            node->flag = true;
        }

        bool search(string word)
        {
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    return false;
                }
                node = node->links[index];
            }

            return node->flag == true; // IMP
        }

        bool startsWith(string word)
        {
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    return false;
                }
                node = node->links[index];
            }

            return true; // path exists = valid prefix, regardless of end of the word
        }

};


int main()
{
    Trie trie;

    trie.insert("apple");

    cout << trie.search("apple") << '\n'; // T
    cout << trie.search("app") << '\n'; // F
    cout << trie.startsWith("apl") << '\n'; // T
    cout << trie.startsWith("bar") << '\n'; // F

    return 0;

}