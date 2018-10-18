/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : fourSum.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/23
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
//思路和3个数的和类似。

class Solution {
public:

    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> res;
        if (nums.size() <= 3)
            return res;
            
        sort(nums.begin(), nums.end());
        int a = 0;
        int b = 0;
        int i = 0;
        int j = 0;
        int len = nums.size();
        int sum = 0;
        vector<int> aidVec;
        
        for (a = 0; a < len - 3; a++)
        {
            for (b = a+1; b < len - 2; b++)
            {
                i = b+1;
                j = len - 1; 
                while(i < j)
                {
                    sum = nums[a] + nums[b] + nums[i] + nums[j];
                    if (sum == target)
                    {
                        aidVec.push_back(nums[a]);
                        aidVec.push_back(nums[b]);
                        aidVec.push_back(nums[i]);
                        aidVec.push_back(nums[j]);
                        if (find(res.begin(), res.end(), aidVec) == res.end())          //怕这里太慢所以加了下面两个循环
                            res.push_back(aidVec); //{nums[a], nums[b], nums[i], nums[j]});
                        aidVec.clear();
                        i++;
                        j--;
                    }
                    else if (sum > target)
                    {
                        j--;
                    }
                    else
                    {
                        i++;
                    }
/*                    
                    while(nums[j] == nums[j-1] && j-1 > i)  //这两个循环可以不要，感觉能加快点速度所以加上，但是会引入各种错误
                    {
                        j--;
                    }
                    while(nums[i] == nums[i+1] && i + 1 < j)
                    {
                        i++;
                    }
*/
                }
            }
        }
        return res;
    }
};
