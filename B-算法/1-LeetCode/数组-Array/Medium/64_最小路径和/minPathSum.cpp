/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : minPathSum.cpp
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
    int minPathSum(vector<vector<int>>& grid) {
        if (grid.size() == 0)
            return 0;
            
        long sum = 0;
        int i = 0;
        int j = 0;
        
        int lenn = grid.size();
        int lenm = grid[0].size();
        for (i = 1, j = 0; i < lenn; i++)
        {
            grid[i][j] = grid[i][j] + grid[i-1][j];
        }
        for (i = 0; i < lenn; i++)
        {
            for (j = 1; j < lenm; j++)
            {
                if (i == 0)
                {
                    grid[i][j] = grid[i][j] + grid[i][j-1];
                }
                else
                {
                    if (grid[i-1][j] > grid[i][j-1])
                    {
                        grid[i][j] += grid[i][j-1];
                    }
                    else
                    {
                        grid[i][j] += grid[i-1][j];
                    }
                }
            }
        }
        
        return grid[lenn-1][lenm-1];
    }
};
