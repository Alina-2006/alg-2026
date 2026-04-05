#include <fstream>
#include <random>
#include <vector>
#include <string>
using namespace std;

void generate_matrix(int n, const string& filename, bool symmetric=true){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);
    vector<vector<int>> mat(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (i == j){
                mat[i][j] = 0;
            } else if (symmetric && i > j){
                mat[i][j] = mat[j][i];
            } else{
                int val = dist(gen);
                mat[i][j] = val;
                if (symmetric) mat[j][i] = val;
            }
        }
    }
    ofstream out(filename);
    out << n << "\n";
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            out << mat[i][j];
            if (j + 1 < n) out << " ";
        }
        out << "\n";
    }
}