题目地址：https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
题目描述：已知数组中的 n 个数是这 n 天股票交易价格，这 n 天只买入一次和卖出一次，求最大利润。
思路：
1. 用一个变量 min 存最小的数，用一个变量存结果 res。
2. 用 i 遍历，当[i]< min 时，min = [i].
3. 当[i] > min 时，计算利润 ([i] - min) 是否大于 res.如果大于则 res = [i] - min.
时间复杂度为 O(n);
