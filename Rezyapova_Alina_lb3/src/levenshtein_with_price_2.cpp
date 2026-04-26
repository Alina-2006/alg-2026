#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void levenshtein_restore(const string& s, const string& t, int rep_cost, int ins_cost, int del_cost){
    int n = s.length();
    int m = t.length();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    
    for (int j = 0; j <= m; j++){
        dp[0][j] = j * ins_cost;
    } 
    for (int i = 0; i <= n; i++){
        dp[i][0] = i * del_cost;
    }

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            int del = dp[i-1][j] + del_cost;
            int ins = dp[i][j-1] + ins_cost;
            int rep = dp[i-1][j-1] + (s[i-1] != t[j-1] ? rep_cost : 0);
            dp[i][j] = min({del, ins, rep});
        }
    }

    int i = n, j = m;
    string ops;

    while (i > 0 || j > 0){
        if (i > 0 && j > 0 && s[i-1] == t[j-1] && dp[i][j] == dp[i-1][j-1]){
            ops.push_back('M');
            i--; j--;
        } else if(i > 0 && j > 0 && dp[i][j] == dp[i-1][j-1] + rep_cost && s[i-1] != t[j-1]){
            ops.push_back('R');
            i--; j--;
        } else if(i > 0 && dp[i][j] == dp[i-1][j] + del_cost){
            ops.push_back('D');
            i--;
        } else if(j > 0 && dp[i][j] == dp[i][j-1] + ins_cost){
            ops.push_back('I');
            j--;
        }
    }
    reverse(ops.begin(), ops.end());

    cout << ops << endl;
    cout << s << endl;
    cout << t << endl;
}

int main(){
    int rep, ins, del;
    cin >> rep >> ins >> del;
    
    string s, t;
    cin >> s >> t;

    levenshtein_restore(s, t, rep, ins, del);
    
    return 0;
}