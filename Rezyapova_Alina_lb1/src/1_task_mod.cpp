#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

//модифицированное задание

int M, N;
int best = 1e9;
int variants = 0;
vector<vector<int>> board;

bool can_place(int x, int y, int size){
    if (x + size > M || y + size > N) return false;
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
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == 0) return {i, j};
    return {-1, -1};
}

void backtrack(int count){
    if (count > best) return;

    auto [x, y] = find_empty();
    if (x == -1){
        if (count < best){
            cout << "Новый минимум:" << count << endl;
            best = count;
            variants = 1;
        }
        else if (count == best){
            cout << "Ещё один вариант с " << count << " квадратами" << endl;
            variants++;
        }
        cout << "Всего вариантов: " << variants << endl;
        return;
    }
    int max_size = min(M - x, N - y);
    int max_allowed = min(M, N) - 1;
    max_size = min(max_size, max_allowed);

    cout << "Ставим квадрат в (" << x << "," << y << "), max_size=" << max_size << ", count=" << count << endl;
    
    for (int size = max_size; size >= 1; size--){
        if (can_place(x, y, size)){
            place(x, y, size, count + 1);
            backtrack(count + 1);
            cout << "  [ОТКАТ] удаляем size=" << size << " из (" << x << "," << y << "), возврат на уровень count=" << count << endl;
            place(x, y, size, 0);
        } else{
            cout << "  -> size=" << size << " не подходит" << endl;
        }
    }
    cout << "[ВОЗВРАТ] на уровень выше из (" << x << "," << y << "), count=" << count << endl;
}

int main(){
    cin >> M >> N;

    if (M <= 1 || N <= 1 || M > 20 || N > 20) {
        cout << "Нет решения (0 0)" << endl; 
        return 0;
    }

    board.assign(M, vector<int>(N, 0));
    best = M * N + 1;
    variants = 0;

    backtrack(0);

    cout << best << " " << variants << endl;
    return 0;
}
