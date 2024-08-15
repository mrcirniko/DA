#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "main.cpp"

bool cmp(my_pair a, my_pair b) {
    return a.key < b.key;
}

int main() {
    std::ofstream out("outfile.txt", std::ios::app);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> key_dist;
    std::uniform_int_distribution<> char_dist(0, 25);
    for (int k = 1; k <= 100000; k *= 10) {

        out << k << '\t';
        std::vector<my_pair> benchmark_data;
        my_vector<my_pair> arr(k);
        my_pair pair;
        for (int i = 0; i < k; ++i) {
            pair.key = key_dist(gen);
            for (int j = 0; j < SIZE_OF_STRING - 1; ++j) {
                pair.value[j] = static_cast<char>('a' + char_dist(gen));
            }
            pair.value[SIZE_OF_STRING - 1] = '\0';
            arr.push_back(pair);
            std::cout << k << '\n';
            benchmark_data.push_back(pair);
        }

        auto start1 = std::chrono::high_resolution_clock::now();
        bucket_sort(arr);
        auto finish1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
        out << duration1.count() << '\t';

        auto start2 = std::chrono::high_resolution_clock::now();
        sort(benchmark_data.begin(), benchmark_data.end(), cmp);
        auto finish2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);

        out << duration2.count() << '\t';

        auto start3 = std::chrono::high_resolution_clock::now();
        std::stable_sort(benchmark_data.begin(), benchmark_data.end(), cmp);
        auto finish3 = std::chrono::high_resolution_clock::now();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(finish3 - start3);

        out << duration3.count() << '\t';
        out << '\n';

    }
    return 0;
}
