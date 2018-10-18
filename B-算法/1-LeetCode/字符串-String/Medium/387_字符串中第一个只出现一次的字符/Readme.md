题目地址：https://leetcode.com/problems/first-unique-character-in-a-string/  
题目描述：返回字符串中第一个只出现一个的字符串的下标。
例如：leetcode，返回0， loveleetcode， 返回2.
思路：用hash表。第一遍记录字符串出现次数到hash表，第二表遍历字符串找hash表中值为1的。
