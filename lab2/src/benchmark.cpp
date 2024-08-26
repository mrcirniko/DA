#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>
#include <vector>
#include <map>

#include "main.cpp"

template <typename KeyT, typename ValueT>
struct test_pair {
    ValueT value;
    KeyT key;
    bool operator<(const test_pair& other) const {
        return key < other.key;
    }
    bool operator>(const test_pair& other) const {
        return key > other.key;
    }
    bool operator==(const test_pair& other) const {
        return key == other.key;
    }
    bool operator!=(const test_pair& other) const {
        return key != other.key;
    }
};

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> key_dist;
    std::uniform_int_distribution<uint8_t> value_dist;
    std::uniform_int_distribution<> char_dist(0, 25);

    for (size_t k = 10000; k <= 10000000; k *= 10) {
        std::cout << k << " & ";
        std::map<uint8_t, uint8_t> map;
        TAVLTree<test_pair<uint8_t, uint8_t>> tree;
        test_pair<uint8_t, uint8_t> data;
        std::vector<test_pair<uint8_t, uint8_t>> vec;

        for (size_t i = 0; i < k; ++i) {
            data.value = key_dist(gen);
            data.key = value_dist(gen);
            vec.push_back(data);
        }

        auto start1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < k; ++i) {
            tree.insert(vec[i]);
            
        }
        for (size_t i = 0; i < k; ++i) {
            tree.remove(vec[i]);    
        }
        auto finish1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(finish1 - start1);
        std::cout << duration1.count() << " & ";

        auto start2 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < k; ++i) {
            map[vec[i].key] = vec[i].value;
            
        }
        for (size_t i = 0; i < k; ++i) {
            map.erase(vec[i].key);  
        }
        auto finish2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(finish2 - start2);
        std::cout << duration2.count() << " \\" << "\\ ";

        std::cout << '\n';
        vec.clear();
        tree.destroy();
    }
}
