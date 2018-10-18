题目地址：https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/
题目描述：这次是不限次数买入和卖出。
思路：和前面一道一样，用min存当前小的数， 用res存结果。不同的是，只要后面的比min大，都累加到res中。
