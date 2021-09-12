/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PascalsTriangleII.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/12
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> res;
        if (rowIndex < 0)
        {
            return res;
        }
        
        vector<int> temp;
        
        int i = 0;
        int j = 0;
        for (i = 0; i <= rowIndex; i++)
        {
            for(j = 0; j <= i; j++)
            {
                if (j == 0 && i == 0)
                {
                    res.push_back(1);
                }
                else if (j == 0)
                {
                    temp.push_back(1);
                }
                else if (j == i)
                {
                    temp.push_back(1);
                    res = temp;
                    temp.clear();
                }
                else
                {
                    temp.push_back(res[j-1] + res[j]);
                }
            }
        }
//        if (rowIndex == 0)
//        {
//            res = temp;
//        }
        
        return res;
    }
};
