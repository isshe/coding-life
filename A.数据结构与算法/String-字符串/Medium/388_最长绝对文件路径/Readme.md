题目地址：https://leetcode.com/problems/longest-absolute-file-path/  
题目描述：返回文件的最长绝对路径。要求时间复杂度:O(n), 空间复杂度 O(n).  
思路：遍历的同时记录路径长度，我的代码中用了递归。  
 
注意：代码还未通过，expected answer 出现问题。百度一个丢上去还是一样的 runtime error。但是试了本地运行可以。  
已通过。因为弄错了题意！  
a\n\rb\n\rc\n\r\rd.e  
理解为：  
a\n\r  
b\n\r  
    c\n\r\r  
    d.e  
    
应为：  
a  
    \n\rb  
    \n\rc  
        \n\r\rd.e  
        
        

