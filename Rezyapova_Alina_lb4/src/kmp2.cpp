#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> culc_prefix_func(const string& S){ // функция поиска префиксов
    int m = S.length();
    vector<int> pi(m, 0);

    for (int i = 1; i < m; i++){
        int j = pi[i-1];
        while (j > 0 && S[i] != S[j]){
            j = pi[j-1];
        }
        if (S[i] == S[j]){
            j++;
        }
        pi[i] = j;
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

    int j = 0;
    for (int i = 0; i < 2 * n - 1; i++){ // идем до 2*n-1, чтобы не выйти за границы
        while(j > 0 && AA[i] != pattern[j]){
            j = pi[j-1];
        }
        if (AA[i] == pattern[j]){
            j++;
        }
        if (j == m){
            int pos = i - m + 1; 
            if (pos < m){ // позиция должна быть исходной длине
                return pos;
            }
        }
    }
    return -1;
}

int main(){
    string A, B;
    getline(cin, A);
    getline(cin, B);

    cout << "строка A = " << A << endl;
    cout << "строка B = " << B << endl;

    cout << "Первое вхождение: " << first_occurrence(A, B) << endl;
    
    return 0;
}