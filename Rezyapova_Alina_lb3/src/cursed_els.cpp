#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

int levenshtein_cursed(const string& s, const string& t, int rep_cost, int ins_cost, int del_cost, const set<int>& cursed_indices){
    int n = s.length();
    int m = t.length();

    auto can_delete = [&](int idx) -> bool{
        if (cursed_indices.count(idx) == 0) return true;
        return s[idx] == 'U'; // из проклятых только 'U' можно удалять 
    };

    auto can_replace = [&](int idx) -> bool{
        if (cursed_indices.count(idx) == 0) return true;
        return false; // если проклят, заменять в любом случае нельзя
    };

    const int INF = 1e9;
    vector<int> prev(m + 1, INF);
    vector<int> curr(m + 1, INF);
    vector<string> table_lines;

    for (int j = 0; j <= m; j++){
        prev[j] = j * ins_cost;
    }

    stringstream ss;
    ss << "        ";
    for (int j = 0; j <= m; j++) {
        if (j == 0) ss << "   ";
        else ss << "   " << t[j-1];
    }
    table_lines.push_back(ss.str());
    ss.str("");

    ss << "i=0    ";
    for (int j = 0; j <= m; j++) {
        ss << setw(4) << prev[j];
    }
    table_lines.push_back(ss.str());
    ss.str("");

    for (int i = 1; i <= n; i++){
        cout << "\nсимвол s[" << i << "] = " << s[i-1] << endl;
        cout << "Проклят? " << (cursed_indices.count(i-1) ? "да" : "нет") << endl;
        cout << "Можно удалять? " << (can_delete(i-1) ? "да" : "нет") << endl;
        cout << "Можно заменять? " << (can_replace(i-1) ? "да" : "нет") << endl;

        if (can_delete(i-1)){
            curr[0] = prev[0] + del_cost;
        } else{
            curr[0] = INF;
        }

        for (int j = 1; j <= m; j++){
            curr[j] = INF;
            if (can_delete(i-1)){
                curr[j] = min(curr[j], prev[j] + del_cost);
            }
            
            curr[j] = min(curr[j], curr[j-1] + ins_cost);
            
            if (can_replace(i-1)){
                int rep = prev[j-1] + (s[i-1] != t[j-1] ? rep_cost : 0);
                curr[j] = min(curr[j], rep);
            }

            else if (s[i-1] == t[j-1]){
                curr[j] = min(curr[j], prev[j-1]); // если замена запрещена, но символы совпали
            }
        }
        ss << "i=" << i << "(" << s[i-1] << ") ";
        for (int j = 0; j <= m; j++) {
            if (curr[j] >= INF/2) ss << "   ∞";
            else ss << setw(4) << curr[j];
        }
        table_lines.push_back(ss.str());
        ss.str("");
        swap(prev, curr);
    }
    if (prev[m] >= INF){
        return -1; // ситуации, когда нельзя ни заменить, ни удалить
    }
    cout << "\nТаблица ДП\n";
    for (const string& line : table_lines) {
        cout << line << endl;
    }
    return prev[m] ;
}

int main(){
    int rep, ins, del;
    cin >> rep >> ins >> del;

    string s, t;
    cin >> s >> t;

    int k;
    cin >> k;
    set<int> cursed_indices;
    for (int i = 0; i < k; i++){
        int pos;
        cin >> pos;
        cursed_indices.insert(pos - 1);
    }

    int result = levenshtein_cursed(s, t, rep, ins, del, cursed_indices);

    if (result == -1){
        cout << "Impossible" << endl; 
    } else{
        cout << result << endl;
    }

    return 0;
}