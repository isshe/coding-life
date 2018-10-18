题目地址：https://leetcode.com/problems/unique-binary-search-trees/    
题目描述：给一个数n， 求1~n，可以构成的唯一的二叉搜索树的条数。  
题目思路：动态规划。  
1. 当n = 0时：res[0] = 1；  
2. 当n = 1时，res[1] = 1；  
3. 当n = 2时，res[2] = res[0]*res[1] + res[1] * res[0];(1为根的情况 + 2为根的情况)  
4. 当n = 3时，  
res[3] =   
res[0]*res[2]   			//根为1的情况  
+ res[1] * res[1] 			//根为2的情况  
+ res[2] * res[0]; 			//根为3的情况  
