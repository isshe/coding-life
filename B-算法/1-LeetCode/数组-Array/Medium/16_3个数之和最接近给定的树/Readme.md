题目地址：https://leetcode.com/problems/3sum-closest/  
题目描述：给一个数组和一个数字target，输出3个数相加的和sum最接近target的sum。
思路：和15题类似。
1. 固定i（在外层循环）
2. j=i+1， k=len-1, 这两个变动。
3. 当sum大于target的时候，k--；
4. 当sum小于target的时候，j++；
5. 同时用一个res保存最接近target的三数之和。
