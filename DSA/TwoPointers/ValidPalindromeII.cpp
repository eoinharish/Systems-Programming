// Given a string s, return true if the s can be palindrome after deleting at most one character from it.

class Solution {
public:

    bool isPalindrome(int L, int R, string s)
    {
        while (L < R)
        {
            if (s[L] != s[R])
            {
                return false;
            }
            L++;
            R--;
        }
        return true;
    }

    bool validPalindrome(string s)
    {
        int n = s.length();
        int cnt = 0;

        int L = 0;
        int R = n-1;

        while(L < R)
        {
            if (s[L] != s[R])
            {   
                // IMP
                // remove either L or R, then one of [L+1,R] or [L, R-1]has to be a palindrome
                return isPalindrome(L+1, R, s) || isPalindrome(L, R-1, s);
            }
            else
            {
                L++;
                R--;
            }
        }
        return true;
        
    }
};

// TC: O(n) [O(2n)]