int titleToNumber(char* s) {
    int len = strlen(s);
    
    int sum = 0;
    int i = 0;
    int j = 0;
    
    for (i = len - 1, j = 0; i>= 0; i--, j++)
    {
        sum = sum + (s[i] - 'A' + 1) * pow(26, j);
    }
    
    return sum;
    
}
