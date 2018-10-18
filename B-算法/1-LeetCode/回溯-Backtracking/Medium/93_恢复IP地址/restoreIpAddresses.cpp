class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        string aidstr = "";
        
        backtracking(s, 0, 0, res, aidstr);         //第一个0代表字符串开始位置， 第二个0代表递归深度
        return res;
    }
    
    void backtracking(string &s, int start, int depth, vector<string> &res, string &aidstr)
    {
        if (depth > 3 || start > 11)
            return ;
        
        int i = 0;
        string str = "";
        int strToInt = 0;
        char *endptr = NULL;
        string saveaid = aidstr;
        
        for (i = 0; i < 3 && (start+i < s.size()); i++)
        {
            str += s[start+i]; 
            
            if ((strToInt = strToi(str)) < 256)
            {
                if (depth == 3 && (start+i == s.size()-1))
                {
                    aidstr += str;
                    res.push_back(aidstr);
                }
                else
                {
                    aidstr += str+".";
                    backtracking(s, start+i+1, depth+1, res, aidstr);
                }
            }
            else
            {
                aidstr = saveaid;
                break;
            }
            aidstr = saveaid;
            if (s[start] == '0')
                break;
        }
    }
    
    int  strToi(string &str)
    {
        int len = str.size();
        int i = 0;
        int res = 0;
        
        for (i = 0; i < len; i++)
        {
            res = res * 10 + str[i]-'0';
        }
        return res;
    }
    
};
