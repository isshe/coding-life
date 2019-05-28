class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res;
        
        res.push_back(0);
        int i = 0;
        int j = 0;
        int len = 0;
        int hightestBitValue = 0;
        
        for (i = 0; i < n; i++)
        {
            hightestBitValue = 1 << i;
            len = res.size();
            for (j = len-1; j >= 0; j--)
            {
                res.push_back(hightestBitValue + res[j]);
            }
        }
        
        return res;
    }
};