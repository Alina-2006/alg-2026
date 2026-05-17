#include <bits/stdc++.h>
using namespace std;

const int K = 5;
int idx(char c) { return c == 'A' ? 0 : c == 'C' ? 1 : c == 'G' ? 2 : c == 'T' ? 3 : 4; }

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    
    string T; int n;
    cin >> T >> n;
    
    vector<string> P(n);
    for (int i = 0; i < n; i++) cin >> P[i];
    
    cout << "Построение бора\n";
    
    vector<array<int,5>> nxt(1);
    fill(nxt[0].begin(), nxt[0].end(), -1);
    vector<int> link(1, -1);
    vector<int> output(1, -1);    
    vector<int> term(1, -1);
    vector<int> len(1, 0);        
    
    for (int i = 0; i < n; i++) {
        int v = 0;
        cout << "Добавление образца " << i+1 << ": \"" << P[i] << "\"\n";
        for (char c : P[i]) {
            int ch = idx(c);
            if (nxt[v][ch] == -1) {
                nxt[v][ch] = nxt.size();
                nxt.emplace_back();
                fill(nxt.back().begin(), nxt.back().end(), -1);
                link.push_back(-1);
                output.push_back(-1);
                term.push_back(-1);
                len.push_back(0);
                cout << "  Новая вершина " << nxt[v][ch] << " по символу " << c << " из " << v << "\n";
            }
            v = nxt[v][ch];
        }
        term[v] = i;
        len[v] = P[i].size();
        cout << "  Вершина " << v << " терминальная (образец " << i+1 << ", длина " << len[v] << ")\n";
    }
    
    cout << "\nБор\n";
    for (int v = 0; v < (int)nxt.size(); v++) {
        cout << v << ": ";
        for (int c = 0; c < K; c++) {
            if (nxt[v][c] != -1) {
                cout << "ACGTN"[c] << "->" << nxt[v][c] << " ";
            }
        }
        if (term[v] != -1) cout << "[образец " << term[v]+1 << "]";
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
            
            if (term[link[u]] != -1) {
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
        if (term[v] != -1) {
            cout << "  Образец: " << term[v]+1 << " (длина " << len[v] << ")\n";
        }
    }
    
    auto go = [&](int v, int c) {
        while (v != -1 && nxt[v][c] == -1) v = link[v];
        return (v == -1) ? 0 : nxt[v][c];
    };
    
    cout << "\nПроцесс поиска\n";
    cout << "Текст: " << T << "\n";
    
    struct Entry { int pos, id, length; };
    vector<Entry> entries;
    
    int v_state = 0;
    for (int i = 0; i < (int)T.size(); i++) {
        char ch = T[i];
        int old = v_state;
        v_state = go(v_state, idx(ch));
        cout << "Шаг " << i+1 << ": символ '" << ch << "' переход " << old << " -> " << v_state << "\n";
        
        if (term[v_state] != -1) {
            int start = i - len[v_state] + 1;
            cout << "  Найден образец " << term[v_state]+1 << " на позиции " << start+1 << "\n";
            entries.push_back({start, term[v_state], len[v_state]});
        }
        
        int u = output[v_state];
        while (u != -1) {
            if (term[u] != -1) {
                int start = i - len[u] + 1;
                cout << "  Найден образец " << term[u]+1 << " на позиции " << start+1 << " (по конечной ссылке)\n";
                entries.push_back({start, term[u], len[u]});
            }
            u = output[u];
        }
    }
    
    cout << "\nРезультаты поиска\n";
    vector<pair<int,int>> ans;
    for (const auto& e : entries) {
        ans.emplace_back(e.pos + 1, e.id + 1);
    }
    sort(ans.begin(), ans.end());
    for (auto [pos, id] : ans) cout << pos << " " << id << "\n";
    
    cout << "\nВариант 2\n";
    cout << "Количество вершин в автомате: " << nxt.size() << "\n";
    
    if (entries.empty()) {
        cout << "Пересекающиеся образцы: нет\n";
        return 0;
    }
    
    sort(entries.begin(), entries.end(), 
         [](const Entry& a, const Entry& b) { return a.pos < b.pos; });
    
    cout << "Все найденные вхождения:\n";
    for (const auto& e : entries) {
        cout << "  образец " << e.id+1 << " \"" << P[e.id] << "\" на позиции " << e.pos+1 
             << " (интервал [" << e.pos+1 << ", " << e.pos+e.length << "])\n";
    }
    
    set<int> overlapping_ids;
    for (size_t i = 0; i < entries.size(); i++) {
        int end_i = entries[i].pos + entries[i].length - 1;
        for (size_t j = i + 1; j < entries.size(); j++) {
            if (entries[j].pos <= end_i) {
                overlapping_ids.insert(entries[i].id);
                overlapping_ids.insert(entries[j].id);
                cout << "Пересечение: образец " << entries[i].id+1 << " и " << entries[j].id+1 << "\n";
            } else {
                break;
            }
        }
    }
    
    cout << "Пересекающиеся образцы: ";
    if (overlapping_ids.empty()) {
        cout << "нет\n";
    } else {
        vector<int> sorted_ids(overlapping_ids.begin(), overlapping_ids.end());
        sort(sorted_ids.begin(), sorted_ids.end());
        for (int id : sorted_ids) {
            cout << P[id] << " ";
        }
        cout << "\n";
    }
    
    return 0;
}