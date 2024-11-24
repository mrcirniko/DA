#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <cstring>

#define STRING_SIZE 257

class AVLTreeData {
public:
    char key[STRING_SIZE];
    uint64_t value;
    AVLTreeData() {
        std::fill_n(key, STRING_SIZE, '\0');
        value = 0;
    }
    ~AVLTreeData() {
    }
    bool operator<(const AVLTreeData& other) const {
        return strcmp(key, other.key) < 0;
    }
    bool operator==(const AVLTreeData& other) const {
        return strcmp(key, other.key) == 0;
    }
    bool operator!=(const AVLTreeData& other) const {
        return strcmp(key, other.key) != 0;
    }
    bool operator>(const AVLTreeData& other) const {
        return strcmp(key, other.key) > 0;
    }

    char& operator[](size_t idx) {
        if (idx >= STRING_SIZE) {
            throw std::out_of_range("Index out of range");
        }
        return key[idx];
    }

    const char& operator[](size_t idx) const {
        if (idx >= STRING_SIZE) {
            throw std::out_of_range("Index out of range");
        }
        return key[idx];
    }
    AVLTreeData(const AVLTreeData& other) {
        std::copy(other.key, other.key + STRING_SIZE, key);
        value = other.value;
    }

    AVLTreeData& operator=(const AVLTreeData& other) {
        if (this != &other) {
            std::copy(other.key, other.key + STRING_SIZE, key);
            value = other.value;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const AVLTreeData& data) {
        for (size_t i = 0; i < STRING_SIZE; ++i) {
            os << data[i];
        }
        os << ", " << data.value;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, AVLTreeData& data) {
        is >> data.key;
        for (size_t i = 0; i < STRING_SIZE; i++) {
            if (data.key[i] != '\0') {
                data.key[i] = std::tolower(data.key[i]);
            }
        }
        return is;
    }
};


template <typename T> 
class TAVLTree {
public:
    class TNode {
    public:
        T data;
        int height;
        TNode* left;
        TNode* right;
        TNode(T d) {
            height = 1;
            data = d;
            left = nullptr;
            right = nullptr;
        }
        void destroy() {
            if (this != nullptr) {
                left->destroy();
                right->destroy();
                delete this;
            }
        }
    };
    TNode* root = nullptr;
    int n;
    TAVLTree() : root(nullptr), n(0) { }

    virtual void insert(T x) { root = insertUtil(root, x); }

    virtual void remove(T x) {
        if (root != nullptr) {
            root = removeUtil(root, x);
        }
    }

    TNode* search(const T x) { return searchUtil(root, x); }

    void printTree() { printTreeUtil(root, 0); }

    void clear() {
        if (root != nullptr) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }
        root = nullptr;
    }
    void destroy(TNode*& node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    void destroy() {
        destroy(root);
    }

    void LoadFromFile(std::ifstream& file) {
        clear();
        size_t fileSize;
        if (file.read(reinterpret_cast<char *>(&fileSize), sizeof(size_t))) {
            for (size_t i = 0; i < fileSize; ++i) {
                T data;
                file.read(data.key, STRING_SIZE);
                file.read(reinterpret_cast<char*>(&data.value), sizeof(data.value));
                insert(data);
            }
        }
    }

    void saveToFile(std::ofstream& file) {
        saveToFileUtil(file, root);
    }
    size_t getSize() {
        return getSizeUtil(root);
    }

protected:
    size_t getSizeUtil(const TNode* node) {
        if (node != nullptr) {
            return 1 + getSizeUtil(node->left) + getSizeUtil(node->right);
        }
        return 0;
    }
    void saveToFileUtil(std::ofstream& file, TNode* node) {
        if (node == nullptr) {
            return;
        }
        file.write(node->data.key, STRING_SIZE);
        file.write(reinterpret_cast<const char*>(&node->data.value), sizeof(node->data.value));

        saveToFileUtil(file, node->left);
        saveToFileUtil(file, node->right);
    }

    void printTreeUtil(TNode* head, int space) {
        if (head == nullptr)
            return;
        space += 10;
        printTreeUtil(head->right, space);
        std::cout << '\n';
        for (int i = 10; i < space; i++)
            std::cout << " ";
        std::cout << head->data << "\n";
        printTreeUtil(head->left, space);
    }
    
    TNode* searchUtil(TNode* head, T x) {
        if (head == nullptr)
            return nullptr;
        T k = head->data;
        if (k == x) {
            return head;
        } else if (k > x) {
            return searchUtil(head->left, x);
        } if (k < x) {
            return searchUtil(head->right, x);
        }
        return nullptr;
    }

    int height(TNode* head) {
        if (head == nullptr)
            return 0;
        return head->height;
    }

    TNode* rightRotation(TNode* head) {
        TNode* newhead = head->left;
        head->left = newhead->right;
        newhead->right = head;
        head->height = 1 + std::max(height(head->left), height(head->right));
        newhead->height = 1 + std::max(height(newhead->left), height(newhead->right));
        return newhead;
    }

    TNode* leftRotation(TNode* head) {
        TNode* new_head = head->right;
        head->right = new_head->left;
        new_head->left = head;
        head->height = 1 + std::max(height(head->left), height(head->right));
        new_head->height = 1 + std::max(height(new_head->left), height(new_head->right));
        return new_head;
    }

    TNode* balance(TNode* head) {
        int bal = height(head->left) - height(head->right);
        if (bal > 1) {
            if (height(head->left) >= height(head->right)) {
                return rightRotation(head);
            } else {
                head->left = leftRotation(head->left);
                return rightRotation(head);
            }
        } else if (bal < -1) {
            if (height(head->right) >= height(head->left)) {
                return leftRotation(head);
            } else {
                head->right = rightRotation(head->right);
                return leftRotation(head);
            }
        }
        return head;
    }

    virtual TNode* insertUtil(TNode* head, T x) {
        if (head == nullptr) {
            n += 1;
            TNode* temp = new TNode(x);
            if (temp == nullptr) {
                throw std::bad_alloc();
            }
            return temp;
        }
        if (x < head->data)
            head->left = insertUtil(head->left, x);
        else if (x > head->data)
            head->right = insertUtil(head->right, x);
        head->height = 1 + std::max(height(head->left), height(head->right));
        return balance(head);
    }
    virtual TNode* removeUtil(TNode* head, T x) {
        if (head == nullptr)
            return nullptr;
        if (x < head->data) {
            head->left = removeUtil(head->left, x);
        } else if (x > head->data) {
            head->right = removeUtil(head->right, x);
        } else {
            TNode* r = head->right;
            if (head->right == nullptr) {
                TNode* l = head->left;
                delete (head);
                head = l;
            } else if (head->left == nullptr) {
                delete (head);
                head = r;
            } else {
                while (r->left != nullptr)
                    r = r->left;
                head->data = r->data;
                head->right = removeUtil(head->right, r->data);
            }
        }
        if (head == nullptr)
            return head;
        head->height = 1 + std::max(height(head->left), height(head->right));
        return balance(head);
    }
};



int main() {
    TAVLTree<AVLTreeData> tree;
    AVLTreeData command;
    while (std::cin >> command) {
        TAVLTree<AVLTreeData>::TNode* node;
        if (command[0] == '+') {
            AVLTreeData data;
            std::cin >> data >> data.value;
            node = tree.search(data);
            if (node == nullptr) {
                tree.insert(data);
                std::cout << "OK" << '\n';
            } else {
                std::cout << "Exist" << '\n';
            }
        } else if (command[0] == '-') {
            AVLTreeData data;
            std::cin >> data;
            node = tree.search(data);
            if (node == nullptr) {
                std::cout << "NoSuchWord" << '\n';
            } else {
                tree.remove(data);
                std::cout << "OK" << '\n';
            }
        
        } else if (command[0] == '!') {
            AVLTreeData action;
            std::cin >> action;
            AVLTreeData path;
            std::cin >> path;
            if (action[0] == 's') {   
                std::ofstream file(path.key, std::ios::binary | std::ios::trunc);
                size_t size = tree.getSize();
                file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                if (size > 0) {
                    tree.saveToFile(file);
                }
                std::cout << "OK\n";
                file.close();

            } else {
                std::ifstream file(path.key, std::ios::binary);
                tree.LoadFromFile(file);
                std::cout << "OK\n";
                file.close();
   
            }

        } else {
            node = tree.search(command);
            if (node == nullptr) {
                std::cout << "NoSuchWord" << '\n';
            } else {
                std::cout << "OK: " << node->data.value << '\n';
            }
        }
        //tree.printTree();
    }
    tree.destroy();
}

/*
                        ЭТО ПОСЛЕДНИЙ, РАБОЧИЙ КОД.

                        ДЛЯ ЗАПИСИ В ФАЙЛ И ЧТЕНИЯ ИЗ ФАЙЛА:
                        ! Save /home/hacker/prog/da_labs/lab2/other_way/file.bin
                        ! Load /home/hacker/prog/da_labs/lab2/other_way/file.bin
                        

                        3 ЛАБА:
                        
                        gcov:
                        g++ -g -fprofile-arcs -ftest-coverage -o cont22 cont2.cpp
                        ./cont22
                        gcov cont2.cpp
                        lcov -t "cont22" -o cont22.info -c -d .
                        https://ps-group.github.io/cxx/coverage_gcc
                        
                        gprof:
                        https://eax.me/c-cpp-profiling/
                        g++ -pg cont2.cpp -o a
                        cat infile.txt | ./a > /dev/null
                        cat infile.txt | gprof ./a > profile
                        cat profile

                        valgrind

*/
