/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : UniquePathII.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/18
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
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.size() == 0)
            return 0;
            
        int row = obstacleGrid.size();
        int col = obstacleGrid[0].size();
        int i = 0;
        int j = 0;
        int flagrow = 0;
        int flagcol = 0;
        
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (i == 0 && obstacleGrid[i][j] == 1)
                {
                    flagrow = 1;
                }
                
                if (j == 0 && obstacleGrid[i][j] == 1)
                {
                    flagcol = 1;
                }
                if (i == 0 || j == 0)
                {
                    if ((i == 0 && flagrow == 1) || (j == 0 && flagcol == 1))
                    {
                        obstacleGrid[i][j] = 0;
                    }
                    else
                    {
                        obstacleGrid[i][j] = 1;
                    }
                }
                if (j != 0 && i != 0)
                {
                    if (obstacleGrid[i][j] == 1)
                    {
                        obstacleGrid[i][j] = 0;
                    }
                    else
                    {
                        obstacleGrid[i][j] = obstacleGrid[i-1][j] + obstacleGrid[i][j-1];
                    }
                }
            }
        }
        
        return obstacleGrid[row-1][col-1];
    }
};
