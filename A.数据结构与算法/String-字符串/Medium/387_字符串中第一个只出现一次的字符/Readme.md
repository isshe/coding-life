题目地址：https://leetcode.com/problems/first-unique-character-in-a-string/  
题目描述：返回字符串中第一个只出现一个的字符串的下标。
例如：leetcode，返回 0，loveleetcode，返回 2.
思路：用 hash 表。第一遍记录字符串出现次数到 hash 表，第二表遍历字符串找 hash 表中值为 1 的。
