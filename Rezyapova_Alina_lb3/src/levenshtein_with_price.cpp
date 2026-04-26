#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int levenshtein_distance(const string& s, const string& t, int rep_cost, int ins_cost, int del_cost){
    int n = s.length();
    int m = t.length();

    vector<int> prev(m + 1);
    vector<int> curr(m + 1);

    for (int j = 0; j <= m; j++){ // заполнение мин кол-вом операций
        prev[j] = j * ins_cost; // инициализация 1 строки, из пустой строки только вставками получаем нужную
    }

    for (int i = 1; i <= n; i++){
        curr[0] = i * del_cost; // i - кол-во символов исходной строки, dp[i][0] = стоиомсть удаления i
        for (int j = 1; j <= m; j++){
            int del = prev[j] + del_cost;
            int ins = curr[j - 1] + ins_cost;
            int rep = prev[j - 1] + (s[i - 1] != t[j - 1] ? rep_cost : 0);
            curr[j] = min({del, ins, rep});
        } 
        swap(prev, curr); // переход к следующей строке
    }
    return prev[m];
}

int main(){
    string s, t;
    int del, ins, rep;
    cin >> del >> ins >> rep;
    cin >> s >> t;

    cout << levenshtein_distance(s, t, del, ins, rep) << endl;

    return 0;
}