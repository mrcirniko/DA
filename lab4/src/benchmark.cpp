#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <cassert>

#include "main.cpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution int_dist(100, 1000);
    std::uniform_int_distribution short_dist(1, 10);
    std::uniform_int_distribution<> char_dist(0, 25);
    for (size_t k = 100000; k <= 100000000; k *= 10) {
        std::string s;
        std::string p;
        int n = int_dist(gen);
        for (size_t i = 0; i < n; ++i) {
            p += static_cast<char>('a' + char_dist(gen));
        }
        for (size_t i = 0; i < k; ++i) {
            int flag = short_dist(gen);
            if (flag == 10) {
                s += p;
                i += n;
            }
            s += static_cast<char>('a' + char_dist(gen));
        }
        std::cout << k << " & ";

        auto start1 = std::chrono::high_resolution_clock::now();
        std::vector<size_t> res1;

        std::vector<long long> z = z_algorithm(p + s);
        for (long long i = 1; i < z.size(); ++i) {
            if (z[i] >= n && i - n >= 0) {
                res1.push_back(i - n);
            }
        }
        auto finish1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
        std::cout << duration1.count() << " & ";

        auto start2 = std::chrono::high_resolution_clock::now();
        std::vector<size_t> res2;
        auto it1 = s.begin();
        while (it1 != s.end()) {
            const std::default_searcher searcher(p.begin(), p.end());
            it1 = std::search(it1, s.end(), searcher);
            if (it1 != s.end()) {
                res2.push_back(it1 - s.begin());
                it1 += n;
            }
        }
        auto finish2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);
        std::cout << duration2.count() << " & ";

        auto start3 = std::chrono::high_resolution_clock::now();
        std::vector<size_t> res3;
        auto it2 = s.begin();
        while (it2 != s.end()) {
            const std::boyer_moore_searcher searcher(p.begin(), p.end());
            it2 = std::search(it2, s.end(), searcher);
            if (it2 != s.end()) {
                res3.push_back(it2 - s.begin());
                it2 += n;
            }
        }
        auto finish3 = std::chrono::high_resolution_clock::now();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(finish3 - start3);
        std::cout << duration3.count() << " \\" << "\\";

        assert(res1 == res2);
        assert(res1 == res3);

        std::cout << '\n';

    }
    return 0;
}
