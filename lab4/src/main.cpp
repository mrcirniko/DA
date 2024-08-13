/*
      lab 4: String algorithms. 
      Task description: It is necessary to implement the search for a pattern in a text using the Z-blocks algorithm. The alphabet consists of lowercase Latin letters.

*/

#include <iostream>
#include <vector>

std::vector<long long> z_algorithm(std::string s) {
    long long n = (long long) s.size();
    std::vector<long long> z(n, 0);
    long long l = 0, r = 0;
    for (long long i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = std::min(z[i - l], r - i + 1);
        }

        while (i + z[i] < n && s[i + z[i]] == s[z[i]]) {
            ++z[i];
        }

        if (i + z[i] - 1 > r) {
            r = i + z[i] - 1;
            l = i;
        }
    }
    return z;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::string s; //string (text)
    std::string p; //pattern
    std::cin >> s;
    std::cin >> p;
    std::vector<long long> z = z_algorithm(p + s);
    long long n = p.size();
    for (long long i = 1; i < z.size(); ++i) {
        if (z[i] >= n && i - n >= 0) {
            std::cout << i - n << '\n';
        }
    }
}
