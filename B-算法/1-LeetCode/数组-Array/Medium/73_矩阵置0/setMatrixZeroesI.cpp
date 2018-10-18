//¿Õ¼ä¸´ÔÓ¶ÈÎªO(m+n);

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int row = matrix.size();
        int col = matrix[0].size();
        vector<int> zeroRow(row, 0);
        vector<int> zeroCol(col, 0);
        
        int i = 0;
        int j = 0;
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (matrix[i][j] == 0)
                {
                    zeroRow[i] = 1;
                    zeroCol[j] = 1;
                }
            }
        }
        for (i = 0; i < row; i++)
        {
            if (zeroRow[i] == 1)
                setRow(matrix, i);
        }
        for (i = 0; i < col; i++)
        {
            if (zeroCol[i] == 1)
                setCol(matrix, i);
        }
    }
    
    void setRow(vector<vector<int>>& matrix, int zRow)
    {
        int col = matrix[0].size();
        int i = 0;
        
        for (i = 0; i < col; i++)
        {
            matrix[zRow][i] = 0;
        }
    }
    void setCol(vector<vector<int>>& matrix, int zCol)
    {
        int row = matrix.size();
        int i = 0;
        for (i = 0; i < row; i++)
        {
            matrix[i][zCol] = 0;
        }
    }
};