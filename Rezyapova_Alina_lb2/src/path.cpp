#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "matrix_gen.cpp"

using namespace std;

const int INF = 1e9;

//Точный метод
void exact_method(int n, vector<vector<int>>& cost){
    int full_mask = (1 << n) - 1; // все биты = 1
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));
    dp[1 << 0][0] = 0;

    vector<vector<int>> parent(1 << n, vector<int>(n, -1));

    for (int mask = 0; mask < (1 << n); mask++){
        for (int last = 0; last < n; last++){
            if (dp[mask][last] == INF) continue; //пути нет, ищем следующий

            for (int next = 0; next < n; next++){
                if (mask & (1 << next)) continue; //уже посещено
                if (cost[last][next] == INF) continue; // нет дороги
                int new_mask = mask | (1 << next);
                int new_cost = dp[mask][last] + cost[last][next];

                if (new_cost < dp[new_mask][next]){
                    dp[new_mask][next] = new_cost;
                    parent[new_mask][next] = last; //запомнили, откуда пришли
                }
            }
        }
    }

    int best_cost = INF;
    int last_city = -1;
    for (int i = 0; i < n; i++){
        if (dp[full_mask][i] == INF) continue;
        if (cost[i][0] == INF) continue; // нет пути обратно
        int total = dp[full_mask][i] + cost[i][0];
        if (total < best_cost){
            best_cost = total;
            last_city = i;
        }
    }

    if (best_cost >= INF){
        cout << "no path" << endl;
        return;
    }

    vector<int> path;
    int mask = full_mask;
    int cur = last_city;
    while (cur != 0){
        path.push_back(cur);
        int prev = parent[mask][cur];
        mask &= ~(1 << cur);
        cur = prev;
    }
    path.push_back(0);
    reverse(path.begin(), path.end());
    path.push_back(0);

    cout << "Стоимость: " << best_cost << endl;
    cout << "Путь: ";
    for (int v : path){
        cout << v << " ";
    }
    cout << endl;
}

//Приближенный метод

//алг Прима
vector<vector<int>> build_MST(int n, vector<vector<int>>& cost){
    vector<bool> used(n, false);
    vector<int> min_edge(n, INF);
    vector<int> parent(n, -1);

    min_edge[0] = 0;
    for (int i = 0; i < n; i++){
        int v = -1;
        for (int j = 0; j < n; j++){
            if (!used[j] && (v == -1 || min_edge[j] < min_edge[v])){
                v = j;
            }
        }
        if (min_edge[v] == INF) break;
        used[v] = true;

        for (int to = 0; to < n; to++){
            if (!used[to] && cost[v][to] < min_edge[to]){
                min_edge[to] = cost[v][to];
                parent[to] = v;
            }
        }
    }
    //восстановлние MST из parent
    vector<vector<int>> mst(n);
    for (int i = 1; i < n; i++){ //у 0 нет parent
        if (parent[i] != -1){
            mst[i].push_back(parent[i]);
            mst[parent[i]].push_back(i);
        }
    }
    return mst;
}

void dfs(int v, int p, vector<vector<int>>& mst, vector<int>& order){
    order.push_back(v);
    for (int to : mst[v]){
        if (to != p){
            dfs(to, v, mst, order);
            order.push_back(v);
        }
    }
}

//АЛШ-2
void appox_method(int n, vector<vector<int>>& cost){
    vector<vector<int>> mst = build_MST(n, cost);

    vector<int> order;
    dfs(0, -1, mst, order);

    vector<bool> seen(n, false);
    vector<int> path;
    for (int v : order){
        if (!seen[v]){
            seen[v] = true;
            path.push_back(v);
        }
    }
    path.push_back(0); // замыкание стартовой вершины

    int total_cost = 0;
    for (size_t i = 0; i + 1 < path.size(); i++){
        if (cost[path[i]][path[i+1]] >= INF){
            cout << "Невозможно построить путь (граф несвязный)" << endl;
            return;
        }
        total_cost += cost[path[i]][path[i+1]];
    }
    cout << "Стоимость: " << total_cost << endl;
    cout << "Путь: ";
    for (int v : path){
        cout << v << " ";
    }
    cout << endl;
}

int main(){
    statint n;
    cin >> n;
    //int n = 5;

    //generate_matrix(n, "input.txt", true);

    //ifstream in("input.txt");
    //in >> n;

    vector<vector<int>> cost(n, vector<int>(n));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> cost[i][j]; 
            //in >> cost[i][j];
            if (i != j && cost[i][j] == 0){
                cost[i][j] = INF;
            }
        }
    }

    //in.close();

    cout << "Точный метод (ДП):" << endl;
    exact_method(n, cost);
    
    cout << "Приближённый метод:" << endl;
    appox_method(n, cost);

    return 0;
}