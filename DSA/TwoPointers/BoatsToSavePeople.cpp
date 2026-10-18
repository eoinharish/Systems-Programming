// You are given an array people where people[i] is the weight of the ith person, and an infinite number of boats
// where each boat can carry a maximum weight of limit. 
// Each boat carries at most two people at the same time, provided the sum of the weight of those people is at most limit.

// Return the minimum number of boats to carry every given person.

// Example 1:

// Input: people = [1,2], limit = 3
// Output: 1
// Explanation: 1 boat (1, 2)
// Example 2:

// Input: people = [3,2,2,1], limit = 3
// Output: 3
// Explanation: 3 boats (1, 2), (2) and (3)
// Example 3:

// Input: people = [3,5,3,4], limit = 5
// Output: 4
// Explanation: 4 boats (3), (3), (4), (5)

class Solution {
public:
    int numRescueBoats(vector<int>& people, int limit)
    {
        int n = people.size();

        if (*max_element(people.begin(), people.end()) > limit)
        {
            return -1;
        }

        sort(people.begin(), people.end());

        int L = 0;
        int R = n-1;

        int boats = 0;
        while (L < R)
        {
            if (people[L] + people[R] <= limit)
            {
                // both can fit in, pair them and move both L, R
                L++;
                R--;
            }
            else
            {
                // only 1 can fit (fit heaviest person, hence move R only)
                // Heaviest person need to take the boat alone
                R--;
            }

            boats++;
        }
        
        if (L == R)
        {
            boats++;
        }

        return boats;

    }
};