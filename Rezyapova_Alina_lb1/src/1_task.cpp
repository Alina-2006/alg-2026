#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

//основное задание

int N;
int best = 1e9;
vector<vector<int>> board;
vector<tuple<int, int, int>> best_result;

bool can_place(int x, int y, int size){
    if (x + size > N || y + size > N) return false;
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            if (board[i][j] != 0) return false;
    return true;
}

void place(int x, int y, int size, int val){
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            board[i][j] = val;
}

pair<int, int> find_empty(){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 0) return {i, j};
    return {-1, -1};
}

void backtrack(int count, vector<tuple<int, int, int>>& current_squares){
    if (count > best) return;

    auto [x, y] = find_empty();
    if (x == -1){
        if (count < best) {  
            best = count;
            best_result = current_squares;
        }
        return;
    }
    
    int max_size = min(N - x, N - y);
    max_size = min(max_size, N - 1);
    
    int remaining = 0;
    for (int i = x; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 0) remaining++;
    
    if (max_size > 2 && count + (remaining + max_size * max_size - 1) / (max_size * max_size) >= best) 
        return;
    
    for (int size = max_size; size >= 1; size--){
        if (can_place(x, y, size)){
            place(x, y, size, count + 1);
            current_squares.push_back({x + 1, y + 1, size});
            backtrack(count + 1, current_squares);
            current_squares.pop_back();
            place(x, y, size, 0);
        }
    }
}

pair<int, vector<tuple<int, int, int>>> solve(int n){
    N = n;
    board.assign(N, vector<int>(N, 0));
    best = N * N + 1;
    best_result.clear();
    vector<tuple<int, int, int>> current_squares;
    
    backtrack(0, current_squares);
    
    return {best, best_result};
}

int main(){
    int N;
    cin >> N;

    if (N < 2 || N > 20){
        cout << "Нет решения (0 0)" << endl; 
        return 0;
    }

    auto [k, squares] = solve(N);

    cout << k << endl;
    for (auto [x, y, w] : squares){
        cout << x << " " << y << " " << w << endl;
    }
    return 0;
}

