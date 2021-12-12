/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : GroupAnagramsI.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/26
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
/*
//思路一：建立一个hashTable，关于string和vector<string>，string是key。
//遍历给的字符串数组strs, 在字符串strs[i]对应的key(字符串排序后的结果为key)那里把strs[i]压入vector<string> 
//vector<string> 相当于一个结果，后面遍历hashTable， 一个一个放入结果数组。
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        map<string, vector<string>> hashTable;
        int len = strs.size();
        int i = 0;
        int j = 0;
        string key;
        vector<vector<string>> res;
       
        for (i = 0; i < len; i++)
        {
            key = strs[i];
            std::sort(key.begin(), key.end());
            hashTable[key].push_back(strs[i]);
        }
        
        map<string, vector<string>>::iterator it;
        for (it = hashTable.begin(); it != hashTable.end(); it++)
        {
            res.push_back(it->second);
        }
        
        return res;
    }
};
*/
//思路二：
//建立hashTable,关于<string, int>   //string 是key， int是结果数组的行。
//遍历字符串数组strs, 把strs[i]排序后的结果当key， hashTable中key对应一个行，在那行压入strs[i].
//还未通过，runtime error！

