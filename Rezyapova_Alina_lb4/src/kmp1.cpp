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
        cout << "\ni=" << i << " (P[i]='" << P[i] << "')\n";
        cout << "  начальное j = pi[" << i-1 << "] = " << j << "\n";
        
        while (j > 0 && P[i] != P[j]){ // попытка продолжить
            cout << "  шаг: P[" << i << "]='" << P[i] 
                 << "' != P[" << j << "]='" << P[j] << "'\n";
            cout << "    -> откат: j было " << j << ", стало pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1]; // откат к меньшему префиксу
        }
        if (P[i] == P[j]){ // символы совпали
            cout << "  P[" << i << "]='" << P[i] << "' == P[" << j << "]='" << P[j] << "'\n";
            j++; // увеличение длины префикса
            cout << "    -> увеличиваем j до " << j << "\n";
        } else {
            cout << "  P[" << i << "]='" << P[i] << "' != P[" << j << "]='" << P[j] << "' (j=0)\n";
        }
        
        pi[i] = j;
        cout << "  pi[" << i << "] = " << j << "\n";
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
    cout << "\nпрефикс-функция: ";
    for (int i = 0; i < m; i++) cout << pi[i] << " ";
    cout << "\n\nначало поиска\n";

    vector<int> occurrences;
    int j = 0;

    for (int i = 0; i < n; i++){
        cout << "\ni=" << i << " (T[i]='" << T[i] << "')\n";
        cout << "  текущее j = " << j << "\n";
        
        while (j > 0 && T[i] != P[j]){
            cout << "  шаг: T[" << i << "]='" << T[i] 
                 << "' != P[" << j << "]='" << P[j] << "'\n";
            cout << "    -> откат: j было " << j << ", стало pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (T[i] == P[j]){
            cout << "  T[" << i << "]='" << T[i] << "' == P[" << j << "]='" << P[j] << "'\n";
            j++; // увеличение длины префикса
            cout << "    -> увеличиваем j до " << j << "\n";
        } else {
            cout << "  T[" << i << "]='" << T[i] << "' != P[" << j << "]='" << P[j] << "' (j=0)\n";
        }
        
        if (j == m){ // полное вхождение(нужная строка нашлась)
            int pos = i - m + 1;
            occurrences.push_back(pos); // добавляем индекс начала вхождения
            cout << "  -> нашли вхождение на позиции " << pos << "\n";
            cout << "    -> откат j=" << j << " -> pi[" << j-1 << "]=" << pi[j-1] << " для продолжения\n";
            j = pi[j-1];
        }
    }
    return occurrences;
}

int main(){
    string P, T;
    getline(cin, P);
    getline(cin, T);
    cout << "исходная строка: " << T << endl;
    cout << "подстрока для поиска: " << P << endl;
    vector<int> result = KMP_search(P, T);
    
    cout << "\nрезультат: " << endl;
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