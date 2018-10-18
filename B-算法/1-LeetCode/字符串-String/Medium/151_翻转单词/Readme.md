题目地址：https://leetcode.com/problems/reverse-words-in-a-string/  
题目描述：翻转字符串。要求去掉多余的空格。  
例如： "the sky is blue" --> "blue is sky the"  

思路：  
方法一：用一个辅助空间存去空格后的字符串，再翻转两次。  
时间复杂度O(n), 空间复杂度O(n).  

方法二：整个翻转字符串同时去掉空格，然后翻转字符串内单词。（个人觉得用c语言比较方便，C++不方便)  
时间复杂度O(n), 空间复杂度O(1).  
