#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> char_dist(0, 25);
    for (int j = 10; j < 1e10; j *= 10) {
        std::uniform_int_distribution int_dist(0, j - 1);
        std::cout << j << " & ";
        auto start = std::chrono::high_resolution_clock::now();
        int n = j;
        vector<vector<int>> g(n);
        vector<int> mt(n, -1);
        vector<bool> u(n);
        for (int i = 0; i < n*n; i++) {
            int a = int_dist(gen);
            int b = int_dist(gen);
            g[a].push_back(b);
            g[b].push_back(a);
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

        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        std::cout << duration.count() << " \\" << "\\ ";
        std::cout << '\n';
    }
    return 0;
}
