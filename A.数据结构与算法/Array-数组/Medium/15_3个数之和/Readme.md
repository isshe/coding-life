题目地址：https://leetcode.com/problems/3sum/  
题目描述：给一个数组，找出所有 3 个元素相加==0 的组合。  
思路一：（未成功）[大致代码在 test.cpp 中]  
1. 排序数组  
2. i, j 分别指向第一个和最后一个元素，mid 指向中间元素。  
3. 当[i]+[j]+[mid] = sum 大于 0 的时候 mid--, 小于的时候 mid++;  
出现各种问题，代码也写得不好。  

思路二：  
1. 排序数组  
2. 开始的时候 i, j 分别指向头尾元素，mid=i+1!!!「这里不一样」  
3. 外层循环用 i（相当预固定 i），内循环中，当 sum > 0 时候，j--; 当 sum < 0 的时候 mid++.(同时还可以跳过相同的元素。）  
4. 当内循环 mid < j 不成立时，当前的 i 已经工作完成，i 指向下一个元素，再调整 mid=i+1, j 依然是最后一个元素。  
时间复杂度为 O(n^2)  
