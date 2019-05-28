/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PascalsTriangle.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/04
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
    vector<vector<int>> generate(int numRows) {
        
        vector<vector<int>> res;
        vector<int> tempRow;
        int i = 0;
        int j = 0;
        
        for (i = 0; i < numRows; i++)
        {
            for (j = 0; j < i+1; j++)
            {
                if (j == 0 || j == i)
                {
                    tempRow.push_back(1);
                    if (j == i)
                    {
                        res.push_back(tempRow);
                        tempRow.clear();
                    }
                }
                else
                {
                    tempRow.push_back(res[i-1][j-1] + res[i-1][j]);
                }
            }
        }
        
        return res;
    }
};
