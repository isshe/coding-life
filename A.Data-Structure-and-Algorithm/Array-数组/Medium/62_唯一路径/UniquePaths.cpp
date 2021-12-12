/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : UniquePaths.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/17
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
    int uniquePaths(int m, int n) {
        vector<vector<int> > array(m);
        
        int i = 0;
        for (i = 0; i < m; i++)
        {
            array[i].resize(n);
        }
        
        int j= 0;
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (i == 0 || j == 0)
                {
                    array[i][j] = 1;
                }
                else
                {
                    array[i][j] = array[i-1][j] + array[i][j-1];
                }
            }
        }
        
        return array[m-1][n-1];
    }
};
