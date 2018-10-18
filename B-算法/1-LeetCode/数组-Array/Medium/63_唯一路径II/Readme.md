题目地址：https://leetcode.com/problems/unique-paths-ii/  
题目描述：是题目62的进阶。多的条件是表格中会有阻碍。  
思路：和原来思路差不多。不同的是当第一行和第一列有阻碍时，该位置和该位置后面的路径数都置0.  
还是f(i, j) = f(i-1, j) + f(i, j-1);  
