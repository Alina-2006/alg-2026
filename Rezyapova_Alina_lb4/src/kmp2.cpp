#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> culc_prefix_func(const string& S){ // функция поиска префиксов
    int m = S.length();
    vector<int> pi(m, 0);

    cout << "pi[0] = 0\n";

    for (int i = 1; i < m; i++){
        int j = pi[i-1];
        cout << "\ni=" << i << " (S[i]='" << S[i] << "')\n";
        cout << "  начальное j = pi[" << i-1 << "] = " << j << "\n";
        
        while (j > 0 && S[i] != S[j]){ // попытка продолжить
            cout << "  шаг: S[" << i << "]='" << S[i] 
                 << "' != S[" << j << "]='" << S[j] << "'\n";
            cout << "    -> откат: j было " << j << ", стало pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1]; // откат к меньшему префиксу
        }
        if (S[i] == S[j]){ //совпадение символов
            cout << "  S[" << i << "]='" << S[i] << "' == S[" << j << "]='" << S[j] << "'\n";
            j++; // увеличение длины префикса
            cout << "    -> увеличиваем j до " << j << "\n";
        } else {
            cout << "  S[" << i << "]='" << S[i] << "' != S[" << j << "]='" << S[j] << "' (j=0)\n";
        }
        
        pi[i] = j;
        cout << "  pi[" << i << "] = " << j << "\n";
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

    cout << "\nAA = " << AA << endl;
    cout << "pattern = " << pattern << endl;
    cout << "\nвычисляем префикс-функцию для pattern:\n";
    
    vector<int> pi = culc_prefix_func(pattern);

    cout << "\nпрефикс-функция: ";
    for (int i = 0; i < pattern.length(); i++) {
        cout << pi[i] << " ";
    }
    cout << "\n\nначало поиска pattern в AA\n";

    int j = 0;
    for (int i = 0; i < 2 * m - 1; i++){ // идем до 2*m-1, чтобы не выйти за границы
        cout << "\ni=" << i << " (AA[i]='" << AA[i] << "')\n";
        cout << "  текущее j = " << j << "\n";
        
        while(j > 0 && AA[i] != pattern[j]){
            cout << "  шаг: AA[" << i << "]='" << AA[i] 
                 << "' != pattern[" << j << "]='" << pattern[j] << "'\n";
            cout << "    -> откат: j было " << j << ", стало pi[" << j-1 << "] = " << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (AA[i] == pattern[j]){
            cout << "  AA[" << i << "]='" << AA[i] << "' == pattern[" << j << "]='" << pattern[j] << "'\n";
            j++;
            cout << "    -> увеличиваем j до " << j << "\n";
        } else {
            cout << "  AA[" << i << "]='" << AA[i] << "' != pattern[" << j << "]='" << pattern[j] << "' (j=0)\n";
        }
        
        if (j == m){ 
            int pos = i - m + 1; 
            if (pos < m){ // позиция должна быть в исходной длине
                cout << "  -> нашли! позиция = " << pos << "\n";
                cout << "\nрезультат: " << pos << endl;
                return pos;
            }
        }
    }
    cout << "\nрезультат: -1 (не найдено)" << endl;
    return -1;
}

int main(){
    string A, B;
    getline(cin, A);
    getline(cin, B);

    cout << "строка A = " << A << endl;
    cout << "строка B = " << B << endl;
    cout << "\nпоиск циклического сдвига\n";

    int result = first_occurrence(A, B);
    
    cout << "\nпервое вхождение: " << result << endl;
    
    return 0;
}