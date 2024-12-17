#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
    string str, pat;
    getline(cin, str);
    str += '\0';
    vector <int> p = BuildSuffixArray(str);
    unsigned int i = 0;
    while(cin >> pat) {
        i++;
        vector<int> res = SlowSearch(0, str.size() - 1, str, p, pat);
        std::sort(res.begin(), res.end());
        if (res.size() != 0) {
            std::cout << i << ": ";
        }
        for (size_t j = 0; j < res.size(); j++) {
            cout << res[j] + 1 << (j == res.size() - 1 ? "\n" : ", ");
        }
    }
}
