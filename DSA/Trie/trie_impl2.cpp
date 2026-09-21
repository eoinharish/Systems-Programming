#include <iostream>
#include <string>

using namespace std;

struct Node
{
    Node* links[26] = {nullptr};
    int countEndsWith = 0;
    int countPrefix = 0;

    ~Node()
    {
        for(int i=0; i < 26; i++)
        {
            delete links[i];
        }
    }
};

// -----------------------------------------------------------------------------------
// INSERT, COUNT_WORDS_ENDS_WITH, COUNT_WORDS_STARTS_WITH : 
// Count how many words equal to the word exists, how many words with a prefix exists
// ----------------------------------------------------------------------------------
class Trie
{
    private:
        Node* root;
    
    public:
        Trie()
        {
            root = new Node();
        }

        void insert(string word)
        {
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    node->links[index] = new Node();
                }
                node = node->links[index];
                node->countPrefix++;
            }

            node->countEndsWith++;
        }

        int countWordsEqualTo(string word)
        {
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    return 0;
                }
                node = node->links[index];
            }

            return node->countEndsWith;
        }

        int countWordsStartingWith(string word)
        {
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    return 0;
                }
                node = node->links[index];
            }

            return node->countPrefix;
        }

        void erase(string word)
        {
            // Step 1: Verify the word actually exists as a complete word
            Node* node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                if (node->links[index] == nullptr)
                {
                    return; // path doesn't even exist
                }
                node = node->links[index];
            }

            if (node->countEndsWith == 0)
            {
                return; // path exists, but word was never inserted as a complete word
            }

            // Step 2: now it's safe to actually erase — walk again and decrement
            node = root;

            for(int i=0; i < word.length(); i++)
            {
                int index = word[i] - 'a';
                node = node->links[index];
                node->countPrefix--;
            }
            node->countEndsWith--;
        }

};

int main()
{
    Trie trie;

    trie.insert("apple");

    cout << trie.countWordsEqualTo("apple") << '\n'; // 1
    cout << trie.countWordsStartingWith("app") << '\n'; // 1

    trie.insert("apple");
    trie.insert("apps");


    cout << trie.countWordsEqualTo("apple") << '\n'; // 2
    cout << trie.countWordsEqualTo("apps") << '\n'; // 1

    cout << trie.countWordsStartingWith("app") << '\n'; // 3
    cout << trie.countWordsStartingWith("a") << '\n'; // 3

    return 0;
}