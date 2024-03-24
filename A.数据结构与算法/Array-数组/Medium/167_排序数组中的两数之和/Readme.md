题目地址：https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/  
题目描述：给一个排好序的数组和一个目标数字 target，要求返回数组中两个之和==target 的数字的位置。「注意不是下标，是位置，也就是下标 +1」。  
思路：  
1. 两个指针 i, j 分别指向头尾元素。  
2. 和>target 的时候，j--；  
3. 和<target 的时候，i++;        
  
