题目地址：https://leetcode.com/problems/unique-binary-search-trees/    
题目描述：给一个数 n，求 1~n，可以构成的唯一的二叉搜索树的条数。  
题目思路：动态规划。  
1. 当 n = 0 时：res[0] = 1；  
2. 当 n = 1 时，res[1] = 1；  
3. 当 n = 2 时，res[2] = res[0]*res[1] + res[1] * res[0];(1 为根的情况 + 2 为根的情况)  
4. 当 n = 3 时，  
res[3] =   
res[0]*res[2]   			//根为 1 的情况  
+ res[1] * res[1] 			//根为 2 的情况  
+ res[2] * res[0]; 			//根为 3 的情况  
