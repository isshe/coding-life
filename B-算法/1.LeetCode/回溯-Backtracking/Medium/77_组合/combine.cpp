class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> res;
        vector<int> array;
        vector<int> partRes;
        for (int i = 0; i < n; i++)
        {
            array.push_back(i+1);
        }
        recursive(res, array, partRes, 0, k);
        
        return res;
    }
    
    void recursive(vector<vector<int>> &res, vector<int> &array, vector<int> &partRes, int n, int k)
    {
        if (0 == k)
        {
            res.push_back(partRes);
        }
        int len = array.size();
        int i = 0;
        for (i = n; i < len; i++)
        {
            partRes.push_back(array[i]);
            recursive(res, array, partRes, i+1, k-1);
            partRes.pop_back();
        }
    }
};