class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int row = matrix.size();
        int col = matrix[0].size();
        bool oneRow = true;
        bool oneCol = true;
        int i = 0;
        int j = 0;
        
        for (i = 0; i < row; i++)
        {
            if (matrix[i][0] == 0)
            {
                oneCol = false;             //第一列有0
            }
        }
        
        for (i = 0; i < col; i++)
        {
            if (matrix[0][i] == 0)
            {
                oneRow = false;             //第一行有0
            }
        }
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (matrix[i][j] == 0)
                {
                    matrix[0][j] = 0;
                    matrix[i][0] = 0;
                }
            }
        }
        
        for (i = 1; i < row; i++)
        {
            if (matrix[i][0] == 0)
            {
                setRow(matrix, i);
            }
        }
        for (i = 1; i < col; i++)
        {
            if (matrix[0][i] == 0)
            {
                setCol(matrix, i);
            }
        }
        if (oneRow == false)
        {
            setRow(matrix, 0);
        }
        if (oneCol == false)
        {
            setCol(matrix, 0);
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