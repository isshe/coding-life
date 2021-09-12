题目地址：https://leetcode.com/problems/powx-n/  
题目描述：给一个pow(double x, int n), 求x^n.  
思路：  
1. 主要注意n，正负号，还有是否==INT_MIN;  
2. x的正负号不用多管。x*x的时候正负号会自动改变。  
3. 用移位来加速计算， 不要一个一个n--循环。  
