/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BestTimeToBuyAndSellStock.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/12
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
    int maxProfit(vector<int>& prices) {
        if (prices.size() == 0)
            return 0;
            
        int min = prices[0];
        int res = 0;
        int i = 0;
        int len = prices.size();
        for (i = 1; i < len; i++)
        {
            if (min > prices[i])
            {
                min = prices[i];
            }
            else
            {
                if (prices[i] - min > res)
                {
                    res = prices[i] - min;
                }
            }
        }
        
        return res;
    }
};
