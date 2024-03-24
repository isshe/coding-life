题目地址：https://leetcode.com/problems/generate-parentheses/  
题目描述：给一个整数 n，生成 n 对括号的组合。
例如：n=3
[
    "((()))"
    "(()())"
    "(())()"
    "()(())"
    "()()()"
]

思路：类似二叉树的遍历。
1. 设置左括号数量 left, 右括号数量 right.
2. 如果 left>0，说明还可以加"("，left--
3. 如果 right>0, 说明还可以加")" right--
4. 当 left == 0 && right == 0 时，打印序列。
