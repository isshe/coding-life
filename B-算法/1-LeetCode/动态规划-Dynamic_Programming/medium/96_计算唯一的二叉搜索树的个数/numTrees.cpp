class Solution {
public:
    int numTrees(int n) {
        vector<int> res(n+1);
        
        int i = 0;
        int j = 0;
        
        res[0] = 1;
        
        for (i = 1; i <= n; i++)
        {
            for (j = 0; j < i; j++)
            {
                res[i] += res[i-j-1] * res[j];
            }
        }
        
        return res[n];
    }
};