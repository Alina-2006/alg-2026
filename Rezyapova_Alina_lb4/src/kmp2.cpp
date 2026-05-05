#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> culc_prefix_func(const string& S){ // функция поиска префиксов
    int m = S.length();
    vector<int> pi(m, 0);

    for (int i = 1; i < m; i++){
        int j = pi[i-1];
        while (j > 0 && S[i] != S[j]){ // попытка продолжить
            cout << "  откат: j = pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1]; // откат к меньшему префиксу
        }
        if (S[i] == S[j]){ //совпадение символов
            j++; // увеличение длины префикса
        }
        pi[i] = j;
        cout << "pi[" << i << "] = " << pi[i] << "\n";
    }
    return pi;
}

int first_occurrence(const string& A, const string& B){
    int m = A.length();
    int n = B.length();

    if (m == 0) return 0;
    if (m != n) return -1;

    string AA = A + A;
    string pattern = B;

    vector<int> pi = culc_prefix_func(pattern);

    cout << "\nПрефикс-функция: ";
    for (int i = 0; i < pattern.length(); i++) {
        cout << pi[i] << " ";
    }
    cout << "\n";

    int j = 0;
    for (int i = 0; i < 2 * m - 1; i++){ // идем до 2*m-1, чтобы не выйти за границы
        while(j > 0 && AA[i] != pattern[j]){
            cout << "  i=" << i << " несовпадение, откат j=" << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (AA[i] == pattern[j]){
            j++;
        }
        cout << "i=" << i << " AA[i]=" << AA[i] << " j=" << j;
        if (j == m){ 
            int pos = i - m + 1; 
            if (pos < m){ // позиция должна быть в исходной длине
                cout << " -> Найдено! позиция=" << pos;
                cout << "\nРезультат: "<< endl;
                return pos;
            }
        }
        cout << "\n";
    }
    cout << "\nРезультат: -1 (не найдено)" << endl;
    return -1;
}

int main(){
    string A, B;
    getline(cin, A);
    getline(cin, B);

    cout << "строка A = " << A << endl;
    cout << "строка B = " << B << endl;

    cout << "\nПервое вхождение: \n" << first_occurrence(A, B) << endl;
    
    return 0;
}