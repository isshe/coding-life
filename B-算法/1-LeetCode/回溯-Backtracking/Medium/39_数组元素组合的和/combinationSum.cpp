/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : combinationSum.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/21
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        if (candidates.size() == 0)
            return res;
        
        vector<int> aidVec;
        //这里不用sort()也可以。
        backtracking(candidates, target, res, aidVec, 0);
        
        return res;
    }
    
    void backtracking(vector<int> &candidates, int target, vector<vector<int>> &res, vector<int> &aidVec, int beforeI)
    {
        if (target == 0)        //不会出现target < 0的情况， 也可以把这个if放到循环里面改为if (target > candidates[i]).
        {
            res.push_back(aidVec);
        }
        
        int len = candidates.size();
        for (int i = beforeI; i < len; i++)
        {
            if (target >= candidates[i])
            {
                aidVec.push_back(candidates[i]);
                backtracking(candidates, target - candidates[i], res, aidVec, i);
                aidVec.pop_back();
            }

        }
    }
};
