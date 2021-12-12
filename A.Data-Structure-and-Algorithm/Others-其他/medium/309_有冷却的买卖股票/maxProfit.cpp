/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : maxProfit.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/24
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
        //这是一道DP问题。
        //分配两个辅助数组：
        //buy[今天]：表示到"今天"为止，最后的动作是buy。所以可以分为：
        //1. "今天"什么也不干，但是因为最后动作是buy,延续上次buy的动作为：buy[今天-1];
        //2. "今天"buy了，所以以前必须是已经sell掉了并且冷却了一天。故：sell[今天-2] = cool[今天-1].
        //故buy的DP状态方程为：buy[i] = max(buy[i-1], sell[i-2]-pricep[i]).//今天什么也不干，或今天buy.
        //sell[今天]: 表示到"今天"为止，最后的动走是sell，可以分为：
        //1. "今天"什么也不干，延续上次sell的动作。
        //2. "今天"sell掉了，所以之前需要买，故：buy[i-1]+price[i]; //price[i]表示今天卖掉挣的钱。
        // 故sell的DP状态方程为:sell[i] = max[sell[i-1], buy[i-1]+price[i]);
        vector<int> buy;
        vector<int> sell;
        
        buy.push_back(0-prices[0]);       //第一天，如果操作最后操作是buy，那只能今天买。
        sell.push_back(0);               //第一天，什么没得卖
        int maxbuy = 0;
        int maxsell = 0;
        int i = 0;
        int len = prices.size();
        
        for(i = 1; i < len; i++)
        {
            if (i == 1)
            {
                maxbuy = buy[i-1] > (0-prices[i]) ? buy[i-1] : (0-prices[i]);
            }
            else
            {
                maxbuy = buy[i-1] > (sell[i-2] - prices[i]) ? buy[i-1] : (sell[i-2] - prices[i]);
            }
            maxsell = sell[i-1] > (buy[i-1] + prices[i]) ? sell[i-1] : (buy[i-1] + prices[i]);
            buy.push_back(maxbuy);
            sell.push_back(maxsell);
        }
        
        return (buy[i-1] > sell[i-1] ? buy[i-1] : sell[i-1]);
    }
};
