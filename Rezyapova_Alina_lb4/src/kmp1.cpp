#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> culc_prefix_func(const string& P){
    int m = P.length();
    vector<int> pi(m, 0);

    cout << "pi[0] = 0\n";

    for (int i = 1; i < m; i++){
        int j = pi[i-1];
        while (j > 0 && P[i] != P[j]){ // попытка продолжить
            cout << "  откат: j = pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1]; // откат к меньшему префиксу
        }
        if (P[i] == P[j]){ // символы совпали
            j++; // увеличение длины префикса
        }
        cout << "pi[" << i << "] = " << pi[i] << "\n";
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
    cout << "\nПрефикс-функция: ";
    for (int i = 0; i < m; i++) cout << pi[i] << " ";
    cout << "\n";

    vector<int> occurrences;
    int j = 0;

    for (int i = 0; i < n; i++){
        while (j > 0 && T[i] != P[j]){
            j = pi[j-1];
        }
        if (T[i] == P[j]){
            j++;
        }
        cout << "i=" << i << " T[i]=" << T[i] << " j=" << j;
        if (j == m){ // полное вхождение(нужная строка нашлась)
            occurrences.push_back(i - m + 1); // добавляем индекс начала вхождения
            cout << " -> нашли вхождение на позиции " << i - m + 1;
            j = pi[j-1];
        }
        cout << "\n";
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