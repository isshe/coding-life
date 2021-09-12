题目地址：https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
题目描述：已知数组中的n个数是这n天股票交易价格，这n天只买入一次和卖出一次，求最大利润。
思路：
1. 用一个变量min存最小的数， 用一个变量存结果res。
2. 用i遍历，当[i]< min时， min = [i].
3. 当[i] > min时，计算利润([i] - min)是否大于res.如果大于则res = [i] - min.
时间复杂度为O(n);
