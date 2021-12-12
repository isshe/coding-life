题目地址：https://leetcode.com/problems/maximum-subarray/  
题目描述：求子数组的最大和。  
For example, given the array [-2,1,-3,4,-1,2,1,-5,4],  
the contiguous subarray [4,-1,2,1] has the largest sum = 6.  

思路：  
1. 遍历数组， 当sum < 0的时候，sum = 0,然后重新计数。   
 
