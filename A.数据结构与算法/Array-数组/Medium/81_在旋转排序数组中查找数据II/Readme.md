题目地址：https://leetcode.com/problems/search-in-rotated-sorted-array-ii/  
题目描述：和题目 33 类似，多一个条件是数据可以重复。  
思路：和 33 题类似，只是如果在判断转折点的时候 [mid] == [left],则 left++(为了能判断转折点在哪边);   
