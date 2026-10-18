// 433. Minimum Genetic Mutation

// A gene string can be represented by an 8-character long string, with choices from 'A', 'C', 'G', and 'T'.

// Suppose we need to investigate a mutation from a gene string startGene to a gene string endGene where one mutation is defined as
// one single character changed in the gene string.

// For example, "AACCGGTT" --> "AACCGGTA" is one mutation.
// There is also a gene bank bank that records all the valid gene mutations. A gene must be in bank to make it a valid gene string.

// Given the two gene strings startGene and endGene and the gene bank bank, return the minimum number of mutations needed to mutate
// from startGene to endGene. If there is no such a mutation, return -1.

// Note that the starting point is assumed to be valid, so it might not be included in the bank.

 
// Example 1:

// Input: startGene = "AACCGGTT", endGene = "AACCGGTA", bank = ["AACCGGTA"]
// Output: 1
// Example 2:

// Input: startGene = "AACCGGTT", endGene = "AAACGGTA", bank = ["AACCGGTA","AACCGCTA","AAACGGTA"]
// Output: 2


class Solution {
public:

    vector<string> getNeighbours(string gene)
    {
        vector<string> neighbours; // total neighbours = 8*3 = 24
        vector<char> chars {'A', 'C', 'G', 'T'};

        for(int i=0; i < gene.length(); i++)
        {
            // Take original character
            char original = gene[i];

            // Mutate character
            for(char ch: chars)
            {
                if (ch != original)
                {
                    gene[i] = ch;
                    neighbours.push_back(gene);
                }
            }
            
            // Store back original character
            gene[i] = original;
        }

        return neighbours;
    }

    int minMutation(string startGene, string endGene, vector<string>& bank)
    {
        unordered_set<string> validGene(bank.begin(), bank.end());

        queue<pair<string, int>> q;
        q.push({startGene, 0});

        while (!q.empty())
        {
            string gene = q.front().first;
            int d = q.front().second;
            q.pop();

            if (gene == endGene)
            {
                return d;
            }

            for(auto it: getNeighbours(gene))
            {
                if (validGene.contains(it))
                {
                    q.push({it, d+1});
                    validGene.erase(it);
                }
            }
        }
        return -1;
    }
};