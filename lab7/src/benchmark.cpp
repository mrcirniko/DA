#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <random>

using namespace std;
using ll = long long;

void Task(ll n) {
    vector<ll> steps(n + 1, 0);
    vector<ll> c(n + 1, numeric_limits<ll>::max());
    c[1] = 0; 

    for (ll i = 2; i <= n; ++i) {
        if (c[i - 1] + i < c[i]) {
            c[i] = c[i - 1] + i;
            steps[i] = -1;
        }
        if (i % 2 == 0 && c[i / 2] + i < c[i]) {
            c[i] = c[i / 2] + i;
            steps[i] = 2;
        }
        if (i % 3 == 0 && c[i / 3] + i < c[i]) {
            c[i] = c[i / 3] + i;
            steps[i] = 3;
        }
    }
    vector<string> res;
    ll i = n;
    while (i > 1) {
        if (steps[i] == -1) {
            res.push_back("-1");
            i -= 1;
        } else if (steps[i] == 2) {
            res.push_back("/2");
            i /= 2;
        } else if (steps[i] == 3) {
            res.push_back("/3");
            i /= 3;
        }
    }
}

int main() {
    for (ll n = 10e2; n < 10e8; n *= 10) {
        std::cout << n << " & ";
        auto start = std::chrono::high_resolution_clock::now();
        Task(n);
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        std::cout << duration.count() << " \\" << "\\ ";
        std::cout << '\n';
    }
    return 0;
}
