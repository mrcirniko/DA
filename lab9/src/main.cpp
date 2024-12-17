#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool dfs(int v, vector<bool>& u, vector<int>& mt, const vector<vector<int>>& g) {
    if (u[v]) {
        return false;
    }
    u[v] = true;
    for (int to : g[v]) {
        if (mt[to] == -1 || dfs(mt[to], u, mt, g)) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n);
    vector<int> mt(n, -1);
    vector<bool> u(n);
    int a, b;
    while(cin >> a >> b) {
        g[a - 1].push_back(b - 1);
        g[b - 1].push_back(a - 1);
    }
    for (int i = 0; i < n; ++i) {
        sort(g[i].begin(), g[i].end());
    }
    for (int v = 0; v < n; ++v) {
        fill(u.begin(), u.end(), false);
        dfs(v, u, mt, g);
    }
    vector<pair<int, int>> res;
    for (int i = 0; i < n; ++i) {
        if (mt[i] != -1 && mt[i] < i) {
            res.emplace_back(mt[i] + 1, i + 1);
        }
    }
    sort(res.begin(), res.end());
    cout << res.size() << endl;
    for (const auto& [a, b] : res) {
        cout << a << ' ' << b << endl;
    }

    return 0;
}
