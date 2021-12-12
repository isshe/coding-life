class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        int len = nums.size();
        vector<vector<int>> res;
        vector<int> aid;
        
        sort(nums.begin(), nums.end());
        recursive(nums, 0, aid, res);

        return res;
    }
    
    void recursive(vector<int>&nums, int pos, vector<int>& aid, vector<vector<int>>& res)
    {
        res.push_back(aid);
        
        int i = 0;
        int len = nums.size();
        
        for (i = pos; i < len; i++)
        {
            if (i > pos &&  nums[i] == nums[i-1])   //�����nums[pos]�����ԣ�����122�� 1�͵�һ��2�ȣ���1�͵ڶ���2�ȣ��ظ��ˡ�
            {
                continue;
            }
            
            aid.push_back(nums[i]);
            recursive(nums, i+1, aid, res);
            aid.pop_back();
        }
    
    }
};