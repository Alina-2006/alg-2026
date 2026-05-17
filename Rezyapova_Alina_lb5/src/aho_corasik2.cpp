#include <bits/stdc++.h>
using namespace std;

const int K = 5;
int idx(char c) {
    return c == 'A' ? 0 : c == 'C' ? 1 : c == 'G' ? 2 : c == 'T' ? 3 : 4;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    
    string T, P, wildcard;
    cin >> T >> P >> wildcard;
    char wc = wildcard[0];
    
    int n = T.size();
    int m = P.size();
    
    cout << "Построение бора\n";
    cout << "Текст: " << T << "\n";
    cout << "Шаблон: " << P << "\n";
    cout << "Джокер: " << wc << "\n";
    
    vector<pair<int, string>> segments;
    cout << "\nРазбор шаблона\n";
    for (int i = 0; i < m; i++) {
        if (P[i] != wc) {
            int j = i;
            string seg;
            while (j < m && P[j] != wc) {
                seg += P[j];
                j++;
            }
            segments.push_back({i, seg});
            cout << "Сегмент " << segments.size() << ": \"" << seg << "\" на позиции " << i << "\n";
            i = j - 1;
        }
    }
    
    if (segments.empty()) {
        cout << "Нет фиксированных символов в шаблоне\n";
        return 0;
    }
    
    cout << "\nПостроение автомата для сегментов\n";
    
    vector<array<int,5>> nxt(1);
    fill(nxt[0].begin(), nxt[0].end(), -1);
    vector<int> link(1, -1);
    vector<int> output(1, -1);
    vector<vector<int>> term(1);
    vector<int> seg_len(1, 0);
    
    for (int id = 0; id < (int)segments.size(); id++) {
        const string& s = segments[id].second;
        int v = 0;
        cout << "Добавление сегмента " << id+1 << ": \"" << s << "\"\n";
        for (char ch : s) {
            int c = idx(ch);
            if (nxt[v][c] == -1) {
                nxt[v][c] = nxt.size();
                nxt.push_back(array<int,5>());
                fill(nxt.back().begin(), nxt.back().end(), -1);
                link.push_back(-1);
                output.push_back(-1);
                term.push_back(vector<int>());
                seg_len.push_back(0);
                cout << "  Новая вершина " << nxt[v][c] << " по символу " << ch << " из " << v << "\n";
            }
            v = nxt[v][c];
        }
        term[v].push_back(id);
        seg_len[v] = s.size();
        cout << "  Вершина " << v << " терминальная (сегмент " << id+1 << ", длина " << s.size() << ")\n";
    }
    
    cout << "\nБор\n";
    for (int v = 0; v < (int)nxt.size(); v++) {
        cout << v << ": ";
        for (int c = 0; c < K; c++) {
            if (nxt[v][c] != -1) {
                cout << "ACGTN"[c] << "->" << nxt[v][c] << " ";
            }
        }
        if (!term[v].empty()) cout << "[сегмент " << term[v][0]+1 << "]";
        cout << "\n";
    }
    
    cout << "\nПостроение автомата\n";
    
    queue<int> q;
    for (int c = 0; c < K; c++) {
        if (nxt[0][c] != -1) {
            link[nxt[0][c]] = 0;
            q.push(nxt[0][c]);
            cout << "Суффиксная ссылка " << nxt[0][c] << " -> 0\n";
        }
    }
    
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int c = 0; c < K; c++) {
            int u = nxt[v][c];
            if (u == -1) continue;
            
            int l = link[v];
            while (l != -1 && nxt[l][c] == -1) l = link[l];
            link[u] = (l == -1) ? 0 : nxt[l][c];
            cout << "Суффиксная ссылка " << u << " -> " << link[u] << "\n";
            
            if (!term[link[u]].empty()) {
                output[u] = link[u];
            } else {
                output[u] = output[link[u]];
            }
            if (output[u] != -1) {
                cout << "Конечная ссылка " << u << " -> " << output[u] << "\n";
            }
            
            q.push(u);
        }
    }
    
    cout << "\nАвтомат\n";
    for (int v = 0; v < (int)nxt.size(); v++) {
        cout << "Вершина " << v << ":\n";
        cout << "  Переходы: ";
        for (int c = 0; c < K; c++) {
            if (nxt[v][c] != -1) {
                cout << "ACGTN"[c] << "->" << nxt[v][c] << " ";
            }
        }
        cout << "\n";
        cout << "  Суффиксная ссылка: " << link[v] << "\n";
        cout << "  Конечная ссылка: " << output[v] << "\n";
        if (!term[v].empty()) {
            cout << "  Сегмент: " << term[v][0]+1 << " (длина " << seg_len[v] << ")\n";
        }
    }
    
    auto go = [&](int v, int c) {
        while (v != -1 && nxt[v][c] == -1) v = link[v];
        return (v == -1) ? 0 : nxt[v][c];
    };
    
    cout << "\nПроцесс поиска\n";
    cout << "Текст: " << T << "\n";
    
    vector<int> C(n + 1, 0);
    
    int v_state = 0;
    for (int i = 0; i < n; i++) {
        char ch = T[i];
        int old = v_state;
        v_state = go(v_state, idx(ch));
        cout << "Шаг " << i+1 << ": символ '" << ch << "' переход " << old << " -> " << v_state << "\n";
        
        for (int id : term[v_state]) {
            int start_pos = i - segments[id].second.size() + 1;
            if (start_pos >= 0) {
                int template_start = start_pos - segments[id].first;
                if (template_start >= 0 && template_start + m <= n) {
                    C[template_start]++;
                    cout << "  Сегмент " << id+1 << " найден на позиции " << start_pos+1 
                         << ", увеличиваем C[" << template_start+1 << "] до " << C[template_start] << "\n";
                }
            }
        }
        
        int u = output[v_state];
        while (u != -1) {
            for (int id : term[u]) {
                int start_pos = i - segments[id].second.size() + 1;
                if (start_pos >= 0) {
                    int template_start = start_pos - segments[id].first;
                    if (template_start >= 0 && template_start + m <= n) {
                        C[template_start]++;
                        cout << "  Сегмент " << id+1 << " найден на позиции " << start_pos+1 
                             << " (по конечной ссылке), увеличиваем C[" << template_start+1 << "] до " << C[template_start] << "\n";
                    }
                }
            }
            u = output[u];
        }
    }
    
    cout << "\nМассив счётчиков C\n";
    for (int i = 0; i <= n - m; i++) {
        cout << "C[" << i+1 << "] = " << C[i];
        if (C[i] == (int)segments.size()) {
            cout << " <-- полное совпадение";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты поиска\n";
    for (int i = 0; i <= n - m; i++) {
        if (C[i] == (int)segments.size()) {
            cout << i + 1 << "\n";
        }
    }
    
    return 0;
}