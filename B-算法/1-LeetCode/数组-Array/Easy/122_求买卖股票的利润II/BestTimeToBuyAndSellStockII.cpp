/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : BestTimeToBuyAndSellStockII.cpp
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
        if(prices.size() == 0)
        {
            return 0;
        }
        
        int min = prices[0];
        int res = 0;
        int len = prices.size();
        int i = 0;
        for (i = 1; i < len; i++)
        {
 //           if (prices[i] < min)
 //           {
 //               min = prices[i];
//            }
//            else
            if (prices[i] > min)
            {
                res += prices[i] - min;
//                min = prices[i];
            }
            min = prices[i];            //为了代码简练这么写，按注释的写可能思路更清晰
        }
        
        return res;
    }
};
