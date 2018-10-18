/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : GroupAnagramsII.cpp
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
//思路二：
//建立hashTable,关于<string, int>   //string 是key， int是结果数组的行。
//遍历字符串数组strs, 把strs[i]排序后的结果当key， hashTable中key对应一个行，在那行压入strs[i].

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        map<string, int> hashTable;
        int len = strs.size();
        int i = 0;
        int j = 0;
        string key;
        vector<vector<string>> res;
        vector<string> aid;             //这个是空的，用来为res开辟空间。使得res[]可用。
/*       
        if (len == 0)
        {
            return res;
        }
        else if (len == 1)
        {
            res.push_back(strs);
            return res;
        }
 */       
        for (i = 0; i < len; i++)
        {
            key = strs[i];
            std::sort(key.begin(), key.end());
            if (hashTable.find(key) == hashTable.end())
            {
                hashTable[key] = j;
                j++;
                res.push_back(aid);                     //为res开辟空间，使下面那句可用，否则会runtime error。
            }
            res[hashTable[key]].push_back(strs[i]);
        }
        
        return res;
    }
};
