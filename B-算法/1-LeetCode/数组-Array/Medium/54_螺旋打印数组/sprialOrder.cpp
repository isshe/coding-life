/*=========================================================================\
* Copyright(C)2016 Isshe.
*
* File name    : sprialOrder.cpp
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
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> res ;
        if (matrix.size() == 0)
            return res;
        

        int mrow = matrix.size()-1;
        int ncol = matrix[0].size()-1;
        int row = 0;
        int col = 0;
        int i = 0;
        
        while(row <= mrow && col <= ncol)
        {
            for(i = col; i <= ncol; i++)
            {
                res.push_back(matrix[row][i]);
        
            }
            for(i = row+1; i < mrow; i++)
            {
                res.push_back(matrix[i][ncol]);
            }
            if (row != mrow)
            {
                for(i = ncol; i >= col; i--)
                {
                    res.push_back(matrix[mrow][i]);
                }
            }
            if (col != ncol)
            {
                for(i = mrow-1; i > row; i--)
                {
                    res.push_back(matrix[i][col]);
                }
            }
            
            ncol--;
            mrow--;
            col++;
            row++;
        }
        
        return res;
    }
};
