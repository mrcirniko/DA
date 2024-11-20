#include <iostream>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;

int main() {
    ll n;
    cin >> n;
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
    cout << c[n] << endl;
    for (ll i = 0; i < res.size(); i++) {
        cout << res[i] << (i == res.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}
