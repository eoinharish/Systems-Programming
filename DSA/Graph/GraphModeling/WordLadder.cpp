#include <bits/stdc++.h>
using namespace std;

// Example 1:

// Input: beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
// Output: 5
// Explanation: One shortest transformation sequence is "hit" -> "hot" -> "dot" -> "dog" -> cog", which is 5 words long.

class Solution {
public:

    vector<string> getNeighbours(string word)
    {
        vector<string> neighbours;
        neighbours.reserve(80); // 75 neighbours

        for(int i=0; i < word.length(); i++)
        {
            // Take original character
            char original = word[i];

            // Mutate character
            for(char ch = 'a'; ch <='z'; ch++)
            {
                if (ch != word[i])
                {
                    word[i] = ch;
                    neighbours.push_back(word);
                }
            }

            // Store back original character
            word[i] = original;
        }

        return neighbours;
    }

    int ladderLength(const string& beginWord, const string& endWord, vector<string>& wordList)
    {

        unordered_set <string> validNeighbours(wordList.begin(), wordList.end());

        if (!validNeighbours.contains(endWord))
        {
            return 0;
        }

        unordered_set <string> visited;
        queue <pair<string, int>> q; 
        q.push({beginWord, 1}); // {hit, 1}
        visited.insert(beginWord); // hit

        while (!q.empty())
        {
            string word = q.front().first; // hit
            int d = q.front().second; // 1
            q.pop();

            if (word == endWord)
            {
                return d;
            }

            for(auto it: getNeighbours(word))
            {
                // not vis and is valid
                if (!visited.contains(it) && validNeighbours.contains(it))
                {
                    q.push({it, d+1}); // hot, 2
                    visited.insert(it);
                }
            }
        }

        return 0; // tranformation not possible
        
    }
};

// abc
// _bc, a_c, ab_ : neighbours 25 + 25 + 25

// Space optimized.
// Don't actually need both validNeighbours and visited set. Can use validNeighbours only.
// Consider erasing words from the validNeighbours set.
// Instead of maintaining both validNeighbours and visited, you can remove a word from validNeighbours as soon as you enqueue it. 
// This avoids redundant lookups and keeps the code simpler.


for(auto it: getNeighbours(word))
{
    // is valid
    if (validNeighbours.contains(it))
    {
        q.push({it, d+1});
        validNeighbours.erase(it);
    }
}