class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res(n, vector<int>(n));
        int i = 0;
        int j = 0;
        int k = 0;
        int num = 1;
        int flag = 0;
        
        n--;
        for (k = n / 2; k >= 0; k--)
        {
            i = flag;
            j = flag;
            for(; j <= n; j++)
            {
                res[i][j] = num++;
            }
            
            for (i++, j--; i <= n; i++)
            {
                res[i][j] = num++;
            }
            
            for (i--, j--; j >= flag; j--)
            {
                res[i][j] = num++;
            }
            
            for (i--, j++; i >= flag+1; i--)
            {
                res[i][j] = num++;
            }
            n--;
            flag++;
        }
        
        
        return res;
    }
};