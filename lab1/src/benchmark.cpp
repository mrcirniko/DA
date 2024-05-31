#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>

#include "main.cpp"

bool cmp(my_pair a, my_pair b) {
    return a.key < b.key;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> key_dist(0, 100000);
    std::uniform_int_distribution<> char_dist(0, 25);
    for (int k = 1; k < 1000000000; k *= 10) {
        std::cout << k << '\t';
        std::vector<my_pair> benchmark_data;

        my_vector<size_t> cnt(MAX_KEY);

        for (size_t i = 0; i < cnt.size(); ++i) {
            cnt[i] = 0;
        }

        my_vector<my_pair> arr;
        my_pair pair;
        for (int i = 0; i < k; ++i) {
            pair.key = key_dist(gen);
            pair.value = static_cast<char>('a' + char_dist(gen));
            ++cnt[pair.key];
            arr.push_back(pair);
            benchmark_data.push_back(pair);
        }

        auto start1 = std::chrono::high_resolution_clock::now();
        counting_sort(cnt, arr);
        auto finish1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
        std::cout << duration1.count() << '\t';

        auto start2 = std::chrono::high_resolution_clock::now();
        sort(benchmark_data.begin(), benchmark_data.end(), cmp);
        auto finish2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);

        std::cout << duration2.count() << '\t';

        auto start3 = std::chrono::high_resolution_clock::now();
        std::stable_sort(benchmark_data.begin(), benchmark_data.end(), cmp);
        auto finish3 = std::chrono::high_resolution_clock::now();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(finish3 - start3);

        std::cout << duration3.count() << '\t';
        std::cout << std::endl;

    }
    return 0;
}
