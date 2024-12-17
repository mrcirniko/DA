#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

std::vector<int> BuildSuffixArray(const std::string& s) {
    const int n = s.size();
    if (n == 0) {
        return {};
    }
    const int alphabet = *std::max_element(s.begin(), s.end()) + 1;
    std::vector<int> p(n), cnt(alphabet, 0), c(n), pn(n), cn(n);

    for (int i = 0; i < n; ++i)
        ++cnt[s[i]];
    for (int i = 1; i < alphabet; ++i)
        cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i)
        p[--cnt[s[i]]] = i;
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i - 1]]) ++classes;
        c[p[i]] = classes - 1;
    }

    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; ++i) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0) pn[i] += n;
        }
        cnt.assign(classes, 0);
        for (int i = 0; i < n; ++i)
            ++cnt[c[pn[i]]];
        for (int i = 1; i < classes; ++i)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (p[i] + (1 << h)) % n, mid2 = (p[i - 1] + (1 << h)) % n;
            if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2])
                ++classes;
            cn[p[i]] = classes - 1;
        }
        std::swap(c, cn);
    }
    return p;
}


vector<int> SlowSearch(int l, int r, const string& str, const vector<int>& p, const string& pat) {
    vector<int> res;
    if (l > r) {
        return res;
    }
    int m = (l + r) / 2;
    int cmp = str.compare(p[m], pat.size(), pat);
    if (cmp == 0) {
        res.push_back(p[m]);
        vector<int> lres = SlowSearch(l, m - 1, str, p, pat);
        res.insert(res.end(), lres.begin(), lres.end());
        vector<int> rres = SlowSearch(m + 1, r, str, p, pat);
        res.insert(res.end(), rres.begin(), rres.end());
        return res;
    } else if (cmp < 0) {
        return SlowSearch(m + 1, r, str, p, pat);
    } else {
        return SlowSearch(l, m - 1, str, p, pat);
    }
    return res;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution int_dist(1, 1000);
    std::uniform_int_distribution short_dist(1, 100);
    std::uniform_int_distribution<> char_dist(0, 25);
    for (int k = 1000; k <= 10000000; k *= 10) {
        //int k = 13;
        std::string str;
        std::string pat;
        int n = int_dist(gen);
        for (int i = 0; i < n; ++i) {
            
        }
        for (int i = 0; i < n; ++i) {
            pat += static_cast<char>('a' + char_dist(gen));
        }
        for (int i = 0; i < k; ++i) {
            int flag = short_dist(gen);
            if (flag == 10) {
                str += pat;
                i += n;
            }
            str += static_cast<char>('a' + char_dist(gen));
        }
        std::cout << k << " & " << pat.size() << " & ";

        str += '\0';

        auto start1 = std::chrono::high_resolution_clock::now();
        
        vector <int> p = BuildSuffixArray(str);
        
        auto finish1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
        
        auto start2 = std::chrono::high_resolution_clock::now();
        vector<int> res = SlowSearch(0, str.size() - 1, str, p, pat);
        std::sort(res.begin(), res.end());
        auto finish2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);
        std::cout << duration1.count()  << " & " << duration2.count() << " \\" << "\\ ";
        std::cout << '\n';
    }
}
