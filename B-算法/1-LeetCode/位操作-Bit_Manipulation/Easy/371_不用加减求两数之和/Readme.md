题目地址：https://leetcode.com/problems/sum-of-two-integers/
题目描述：不用加减求两数之和。
思路：每次求一部分位的和。
1. 用 & 求进位。
2. 用 ^ 求和 （不进位的和）
3. 如果不再有进位，则不用再求和。
