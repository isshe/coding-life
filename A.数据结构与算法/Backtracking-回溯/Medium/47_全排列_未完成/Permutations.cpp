class Solution {
public:
    void internalpermuteUnique(vector<int> &num, int index, vector<int> &perm, vector<vector<int> > &result) {
        int size = num.size();
        
        if (size == index) {
            result.push_back(perm);
        }
        else {
            for (int i = index; i < size; ++i) {
                if ((i > index) && (num[i] == num[index])) {
                  continue;
                }
                else {
                  swap(num[index], num[i]);
                }
                
                perm.push_back(num[index]);
                internalpermuteUnique(num, index + 1, perm, result);
                perm.pop_back();
            }
            
            sort(num.begin() + index, num.end());
        }
    }
    
    vector<vector<int> > permuteUnique(vector<int> &num) {
        vector<vector<int> > result;
        vector<int> perm;
        
        sort(num.begin(), num.end());
        internalpermuteUnique(num, 0, perm, result);
        
        return result;
    }
};