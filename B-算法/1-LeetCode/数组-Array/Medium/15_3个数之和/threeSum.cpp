/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : threeSum.cpp
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
    vector<vector<int>> threeSum(vector<int> &nums) {
        vector<vector<int> > result;
        
        // 排序
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int sum = 0;
        
        for (int i = 0; i < n - 2; ++i) {
            // 去重复
            // 下面注释掉的去重复方案是错误的， 会把0，0，0也去掉
//            while (i < n -2 && nums[i] == nums[i+1]) {
//                i++;
//            }
             if (i>0 && nums[i-1] == nums[i]){
                 continue;
             }
            int j = i + 1;
            int k = n - 1;
            while (j < k) {
                sum = nums[i] + nums[j] + nums[k];
                if (sum < 0){
                    // 去重复
                    while (j < k && nums[j] == nums[j+1])  {
                        j++;
                    }
                    j++;
                }
                else if (sum > 0){
                     // 去重复
                    while (k > j && nums[k] == nums[k-1]) {
                        k--;
                    }
                    k--;
                }
                else{
                    result.push_back({nums[i], nums[j], nums[k]});
                    // 去重复
                    while (j < k && nums[j] == nums[j+1]) {
                        j++;
                    }
                    while (k > j && nums[k] == nums[k-1]) {
                        k--;
                    }
                    j++;
                    k--;
                }
            }
        }
        return result;
    }
};
