#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> culc_prefix_func(const string& P){
    int m = P.length();
    vector<int> pi(m, 0);

    for (int i = 1; i < m; i++){
        int j = pi[i-1];
        while (j > 0 && P[i] != P[j]){
            j = pi[j-1]; // откат к меньшему префиксу
        }
        if (P[i] == P[j]){ // символы совпали
            j++; // увеличение длины префикса
        }
        pi[i] = j;
    }
    return pi;
}

vector<int> KMP_search(const string& P, const string& T){
    int m = P.length();
    int n = T.length();

    if (m == 0){
        return {};
    }

    vector<int> pi = culc_prefix_func(P);

    vector<int> occurrences;
    int j = 0;

    for (int i = 0; i < n; i++){
        while (j > 0 && T[i] != P[j]){
            j = pi[j-1];
        }
        if (T[i] == P[j]){
            j++;
        }
        if (j == m){ // полное вхождение(нужная строка нашлась)
            occurrences.push_back(i - m + 1); // добавляем индекс начала вхождения
            j = pi[j-1];
        }
    }
    return occurrences;
}

int main(){
    string P, T;
    getline(cin, P);
    getline(cin, T);
    cout << "Исходная строка: " << T << endl;
    cout << "Подстрока для поиска: " << P << endl;
    vector<int> result = KMP_search(P, T);
    
    cout << "Результат: " << endl;
    if (result.empty()){
        cout << -1 << endl;
    } else {
        for (size_t i = 0; i < result.size(); i++){
            if (i > 0){
                cout << ",";
            }
            cout << result[i];
        }
        cout << endl;
    }
    return 0;
}