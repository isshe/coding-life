题目地址：https://leetcode.com/problems/3sum-closest/  
题目描述：给一个数组和一个数字 target，输出 3 个数相加的和 sum 最接近 target 的 sum。
思路：和 15 题类似。
1. 固定 i（在外层循环）
2. j=i+1，k=len-1, 这两个变动。
3. 当 sum 大于 target 的时候，k--；
4. 当 sum 小于 target 的时候，j++；
5. 同时用一个 res 保存最接近 target 的三数之和。
