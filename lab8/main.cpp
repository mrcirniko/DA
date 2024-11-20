#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct TTriplet {
    std::vector<double> vec;
    int initIdx;
    int price;
};

void GaussWithPriority() {
    int m, n;
    std::cin >> m >> n;
    std::vector<TTriplet> vectors(m, TTriplet{std::vector<double>(n), 0, 0});

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> vectors[i].vec[j];
        }
        std::cin >> vectors[i].price;
        vectors[i].initIdx = i;
    }

    std::vector<int> res;

    for (int i = 0; i < n; ++i) {
        int minVecIdx = -1;
        int minPrice = std::numeric_limits<int>::max();

        for (int j = i; j < m; ++j) {
            if (vectors[j].vec[i] != 0.0 && vectors[j].price < minPrice) {
                minPrice = vectors[j].price;
                minVecIdx = j;
            }
        }

        if (minVecIdx == -1) {
            std::cout << "-1\n";
            return;
        }

        std::swap(vectors[i], vectors[minVecIdx]);
        res.push_back(vectors[i].initIdx);

        for (int j = i + 1; j < m; ++j) {
            double multiplier = vectors[j].vec[i] / vectors[i].vec[i];
            for (int k = i; k < n; ++k) {
                vectors[j].vec[k] -= vectors[i].vec[k] * multiplier;
            }
        }
    }

    std::sort(res.begin(), res.end());
    for (int elem : res) {
        std::cout << elem + 1 << " ";
    }
    std::cout << std::endl;
}

int main() {
    GaussWithPriority();
    return 0;
}
