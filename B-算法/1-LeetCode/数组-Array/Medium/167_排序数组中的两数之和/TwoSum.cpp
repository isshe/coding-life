/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : TwoSum.cpp
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
    vector<int> twoSum(vector<int>& numbers, int target) {
        if (numbers.size() <= 1)
            return numbers;
            
        vector<int> res;
        int i = 0;
        int j = numbers.size() -1;
        int sum = 0;
        while(i < j)
        {
            sum = numbers[i] + numbers[j];
            if ( sum > target)
            {
                j--;
            }
            else if (sum < target)
            {
                i++;
            }
            else
            {
                i++;
                j++;
                break;
            }
        }
        res.push_back(i);
        res.push_back(j);
        return res;
    }
};
