#include <iostream>
#include <limits>
#include <cstring> 

#define SIZE_OF_STRING 65

template <typename T>
class my_vector {
private:
    T * data;
    uint64_t size;
    uint64_t capacity;
public:
    my_vector() : data(nullptr), size(0), capacity(0) {}
    my_vector(uint64_t new_size) {
        data = new T[new_size];
        size = 0;
        capacity = new_size;
    }
    ~my_vector() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }
    uint64_t get_size() {
        return size;
    }
    my_vector(const my_vector& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        std::copy(other.data, other.data + size, data);
    }

    my_vector(my_vector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    my_vector& operator=(const my_vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
    my_vector& operator=(my_vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }
    void push_back(const T& elem) {
        if (size >= capacity) {
            throw std::out_of_range("Index out of range. Size: " + std::to_string(size));
        }
        data[size++] = elem;
    }
    void resize(const int k) {
        if (k <= 0) return;
        capacity += k;
        T* temp = new T[capacity];
        if (data != nullptr) {
            std::copy(data, data + size, temp);
        }
        delete[] data;
        data = temp;
    }
    T& operator[](uint64_t index) {
        return data[index];
    }
};


template<class T>
const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

template<class T>
const T& max(const T& a, const T& b)
{
    return (b > a) ? b : a;
}

template <class T>
void merge(my_vector<T>& arr, my_vector<T>& buf, std::size_t left, std::size_t mid, std::size_t right) {
    std::size_t it1 = 0;
    std::size_t it2 = 0;
    while (left + it1 < mid && mid + it2 < right) {
        if (arr[left + it1] <= arr[mid + it2]) {
            buf[it1 + it2] = std::move(arr[left + it1]);
            it1 += 1;
        } else {
            buf[it1 + it2] = std::move(arr[mid + it2]);
            it2 += 1;
        }
    }
    while (left + it1 < mid) {
        buf[it1 + it2] = std::move(arr[left + it1]);
        it1 += 1;
    }

    while (mid + it2 < right) {
        buf[it1 + it2] = std::move(arr[mid + it2]);
        it2 += 1;
    }
    for (std::size_t i = 0; i < it1 + it2; ++i) {
        arr[left + i] = std::move(buf[i]);
    }
}

template <class T>
void merge_sort(my_vector<T>& arr, my_vector<T>& buf) {
    std::size_t count = arr.get_size();
    for (size_t i = 1; i < count; i *= 2) {
        for (size_t j = 0; j < count - i; j += 2 * i) {
            merge(arr, buf, j, j + i, min(j + 2 * i, count));
        }
    }
}

struct my_pair {
    uint64_t key;
    char value[SIZE_OF_STRING];
    my_pair() {
        std::memset(value, 0, SIZE_OF_STRING);
    }
    bool operator<(const my_pair& other) const {
        return key < other.key;
    }
    bool operator<=(const my_pair& other) const {
        return key <= other.key;
    }
};

void bucket_sort(my_vector<my_pair> & v, uint64_t m) {
    uint64_t n = v.get_size();
    my_vector<my_pair> b[n];
    uint64_t temp1[n];
    int temp[n];
    for (uint64_t i = 0; i < n; ++i) {
        temp[i] = 0;
    }
    for (uint64_t i = 0; i < n; ++i) {
        temp1[i] = n * v[i].key / std::numeric_limits<uint64_t>::max();
        temp[temp1[i]]++;
    }
    for (uint64_t i = 0; i < n; ++i) {
        b[temp1[i]].resize(temp[i]);
        b[temp1[i]].push_back(v[i]);
    }
    for (uint64_t i = 0; i < n; ++i) {
        if (b[i].get_size() > 1) {
            merge_sort(b[i], v);
        }
        for (uint64_t j = 0; j < b[i].get_size(); j++) {
            std::cout << b[i][j].key << "\t" << b[i][j].value << '\n';
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    my_vector<my_pair> v(1000000);
    my_pair p;
    uint64_t m = 0;
    while (std::cin >> p.key >> p.value) {
        p.value[SIZE_OF_STRING - 1] = '\0';
        v.push_back(p);
    }
    bucket_sort(v, m);
    return 0;
}
