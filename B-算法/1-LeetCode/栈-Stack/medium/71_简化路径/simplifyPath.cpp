class Solution {
public:
    string simplifyPath(string path) {
        int i = 0;
        int len = path.size();
        vector<string> aidStack;
        string aidAndRes = "";
        
        for (i = 0; i < len; i++)
        {
            aidAndRes = "";
            if (path[i] == '/')
            {
                continue;
            }
            for(; path[i] != '/' && i < len; i++)
            {
                aidAndRes += path[i];
            }
            
            if ("." == aidAndRes)
            {
                continue;
            }
            else if (".." == aidAndRes)
            {
                if (!aidStack.empty())
                {
                    aidStack.pop_back();
                }
                continue;
            }
            
            aidStack.push_back(aidAndRes);
        }
        
        aidAndRes = "";
        if (aidStack.empty())
        {
            aidAndRes = "/";
        }
        else
        {
            for (i = 0; i < aidStack.size(); i++)
            {
                aidAndRes += "/" + aidStack[i];
            }
        }
        return aidAndRes;
    }
};