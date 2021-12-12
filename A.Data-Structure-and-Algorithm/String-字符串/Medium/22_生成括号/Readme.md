题目地址：https://leetcode.com/problems/generate-parentheses/  
题目描述：给一个整数n，生成n对括号的组合。
例如：n=3
[
    "((()))"
    "(()())"
    "(())()"
    "()(())"
    "()()()"
]

思路：类似二叉树的遍历。
1. 设置左括号数量left, 右括号数量right.
2. 如果left>0，说明还可以加"("，left--
3. 如果right>0, 说明还可以加")" right--
4. 当left == 0 && right == 0时，打印序列。
