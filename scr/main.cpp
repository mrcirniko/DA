#include <iostream>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

const size_t MAX_KEY = 10e5 + 1;

#pragma region Vector
template <typename T>
class my_vector {
    T* _data;
    size_t _size, _capacity;

   public:
    my_vector();
    my_vector(size_t init_size);
    my_vector(const my_vector& other);
    my_vector(my_vector&& other);
    ~my_vector();

    T* Data() const;
    bool is_empty();
    void resize(size_t new_size);
    void push_back(const T& new_elem);

    T* data() const;
    size_t size() const;
    size_t capacity() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    my_vector& operator=(const my_vector& other);
    my_vector& operator=(my_vector&& other);

   private:
    static T* allocate(size_t new_capacity);
    void reallocate(size_t new_capacity);
};

template <typename T>
my_vector<T>& my_vector<T>::operator=(const my_vector &other) {
    _size = other.size();
    _capacity = other.capacity();
    _data = allocate(_capacity);
    std::copy(other.data(), other.data() + _size, _data);
    return *this;
}

template <typename T>
my_vector<T>& my_vector<T>::operator=(my_vector &&other) {
    _size = other.size();
    _capacity = other.capacity();
    _data = other.data();
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    return *this;
}

template <typename T>
size_t my_vector<T>::size() const {
    return _size;
}

template <typename T>
size_t my_vector<T>::capacity() const {
    return _capacity;
}

template <typename T>
T* my_vector<T>::data() const {
    return _data;
}

template <typename T>
T* my_vector<T>::allocate(size_t new_capacity) {
    return new T[new_capacity];
}

template <typename T>
void my_vector<T>::reallocate(size_t new_capacity) {
    T* temp = allocate(new_capacity);
    std::copy(_data, _data + _size, temp);
    _capacity = new_capacity;
    delete[] _data;
    _data = temp;
}

template <typename T>
my_vector<T>::my_vector() : _size(0), _capacity(0), _data(nullptr) {}

template <typename T>
my_vector<T>::my_vector(size_t init_size) : _size(init_size), _capacity(init_size) {
    _data = allocate(_capacity);
}

template <typename T>
my_vector<T>::my_vector(const my_vector& other)
    : _size(other.size()), _capacity(other.capacity()) {
    _data = allocate(_capacity);
    std::copy(other.data(), other.data() + _size, _data);
}

template <typename T>
my_vector<T>::my_vector(my_vector&& other)
    : _size(other._size), _capacity(other._capacity), _data(other._data) {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
my_vector<T>::~my_vector() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
}

template <typename T>
bool my_vector<T>::is_empty() {
    return _size == 0;
}

template <typename T>
void my_vector<T>::resize(size_t new_size) {
    if (new_size <= _capacity) {
        _size = new_size;
    } else {
        size_t new_capacity = _capacity > size_t(1) ? _capacity : size_t(1);
        while (new_capacity < new_size) {
            new_capacity *= 2;
        }
        reallocate(new_capacity);
        _size = new_size;
    }
}

template <typename T>
void my_vector<T>::push_back(const T& new_elem) {
    resize(_size + 1);
    (*this)[_size - 1] = new_elem;
}

template <typename T>
T& my_vector<T>::operator[](size_t index) {
    return _data[index];
}

template <typename T>
const T& my_vector<T>::operator[](size_t index) const {
    return _data[index];
}

#pragma endregion Vector

struct my_pair {
    uint32_t key;
    char value;
};

void counting_sort(my_vector<size_t>& cnt, my_vector<my_pair>& arr) {
    for (size_t i = 1; i < cnt.size(); ++i) {
        cnt[i] = cnt[i - 1] + cnt[i];
    }

    my_vector<my_pair> res(arr.size());

    for (size_t i = arr.size(); i-- > 0;) {
        uint32_t key = arr[i].key;
        res[cnt[key] - 1] = arr[i];
        --cnt[key];
    }
    arr = res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    my_vector<size_t> cnt(MAX_KEY);

    for (size_t i = 0; i < cnt.size(); ++i) {
        cnt[i] = 0;
    }

    my_vector<my_pair> arr;
    my_pair pair;

    while (std::cin >> pair.key >> pair.value) {
        ++cnt[pair.key];
        arr.push_back(pair);
    }
    counting_sort(cnt, arr);

    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i].key << '\t' << arr[i].value << '\n';
    }
    return 0;
}
