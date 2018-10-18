bool searchMatrix(int** matrix, int matrixRowSize, int matrixColSize, int target) {
    int row = matrixRowSize -1;
    int col = matrixColSize -1;
    int i = 0;
    int j = 0;
    
    while(col >= 0 && matrix[0][col] > target)          //这两个条件的位置会有影响
    {
        col--;
    }
    while(row >= 0 && matrix[row][0] > target)
    {
        row--;
    }
    if (col < 0 || row < 0)
        return false;
        
    for (i = row; i >= 0; i--)
    {
        for (j = 0; j <= col; j++)
        {
            if (matrix[i][j] == target)
            {
                return true;
            }
        }
    }
    
    return false;
    
}