题目地址：https://leetcode.com/problems/integer-to-roman/  
题目描述：给一个整数，输出它对应的罗马数字。  
思路：直接列罗马数字到数组中：  
如：  
        string  symble[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};  
        int     value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};  
每次减去相应的数字，打印相应的字符串。  
