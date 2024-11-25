/*
      struct Tnode - modified persistent treap

*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>


template <typename T>
int BinarySearch(std::vector<T>& v, int x) {
    int l = -1;
    int r = v.size();
    if (x < v[0].x || x > v[v.size() - 1].x) {
        return -1;
    }
    while (l + 1 < r) {
        int mid = l + (r - l) / 2;

        if (v[mid].x <= x) {
            l = mid;
        } else {
            r = mid;
        }
    }
    return l;
}

struct TNode {
    TNode* left;
    TNode* right;
    int key;
    int priority;
    int count;
    int sumOfCounts;
};

int sumOfCounts(TNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return root->sumOfCounts;
}

TNode* SearchInTreap(TNode* root, int key) {
    if (root == nullptr) {
        return root;
    } else if (root->key == key) {
        return root;
    } else if (key < root->key) {
        return SearchInTreap(root->left, key);
    } else {
        return SearchInTreap(root->right, key);
    }
}

std::pair<TNode*, TNode*> Split(TNode* root, int key) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    } else if (key < root->key) {
        std::pair<TNode*, TNode*> leftTree = Split(root->left, key);
        TNode* rightTree = new TNode();
        rightTree->right = root->right;
        rightTree->key = root->key;
        rightTree->priority = root->priority;
        rightTree->count = root->count;
        rightTree->left = leftTree.second;
        rightTree->sumOfCounts = sumOfCounts(rightTree->right) + sumOfCounts(rightTree->left) + 1 + rightTree->count;
        return {leftTree.first, rightTree};
    } else {
        std::pair<TNode*, TNode*> rightTree = Split(root->right, key);
        TNode* leftTree = new TNode();
        leftTree->left = root->left;
        leftTree->key = root->key;
        leftTree->priority = root->priority;
        leftTree->count = root->count;
        leftTree->right = rightTree.first;
        leftTree->sumOfCounts = sumOfCounts(leftTree->left) + sumOfCounts(leftTree->right) + 1 + leftTree->count;
        return {leftTree, rightTree.second};
    }
}

TNode* InsertUtil(TNode* root, int key, int priority) {
    TNode* newRoot = new TNode();
    if (root == nullptr || priority > root->priority) {
        std::pair<TNode*, TNode*> splitted = Split(root, key);
        newRoot->left = splitted.first;
        newRoot->right = splitted.second;
        newRoot->key = key;
        newRoot->priority = priority;
        newRoot->sumOfCounts = sumOfCounts(splitted.first) + sumOfCounts(splitted.second) + 1;
        return newRoot;
    }
    newRoot->key = root->key;
    newRoot->priority = root->priority;
    newRoot->sumOfCounts = root->sumOfCounts + 1;
    newRoot->count = root->count;
    if (key < root->key) {
        newRoot->right = root->right;
        newRoot->left = InsertUtil(root->left, key, priority);
    } else {
        newRoot->left = root->left;
        newRoot->right = InsertUtil(root->right, key, priority);
    }
    return newRoot;
}

TNode* CopyNodesWithIncrement(TNode* root, int key) {
    TNode* newRoot = new TNode();
    if (root->key == key) {
        newRoot->left = root->left;
        newRoot->right = root->right;
        newRoot->key = key;
        newRoot->priority = root->priority;
        newRoot->count = root->count + 1;
        newRoot->sumOfCounts = root->sumOfCounts + 1;
        return newRoot;
    }
    newRoot->key = root->key;
    newRoot->priority = root->priority;
    newRoot->sumOfCounts = root->sumOfCounts + 1;
    newRoot->count = root->count;
    if (key < root->key) {
        newRoot->right = root->right;
        newRoot->left = CopyNodesWithIncrement(root->left, key);
    } else {
        newRoot->left = root->left;
        newRoot->right = CopyNodesWithIncrement(root->right, key);
    }
    return newRoot;
}

TNode* Insert(TNode* root, int key, int priority) {
    if (!SearchInTreap(root, key)) {
        return InsertUtil(root, key, priority);
    }
    return CopyNodesWithIncrement(root, key);
}

void PrintTree(TNode* head, int space) {
    if (head == nullptr)
        return;
    space += 10;
    PrintTree(head->right, space);
    std::cout << '\n';
    for (int i = 10; i < space; ++i) {
        std::cout << ' ';
    }
    std::cout << head->key << ", " << head->count << '\n';
    PrintTree(head->left, space);

}

TNode* Merge(TNode* leftTree, TNode* rightTree) {
    if (leftTree == nullptr) {
        return rightTree;
    }
    
    if (rightTree == nullptr) {
        return leftTree;
    }

    TNode* newRoot = new TNode();
    if (leftTree->priority > rightTree->priority) {
        newRoot->key = leftTree->key;
        newRoot->priority = leftTree->priority;
        newRoot->left = leftTree->left;
        newRoot->count = leftTree->count;
        newRoot->right = Merge(leftTree->right, rightTree);
        newRoot->sumOfCounts = sumOfCounts(newRoot->left) + sumOfCounts(newRoot->right) + 1 + newRoot->count;
    } else {
        newRoot->key = rightTree->key;
        newRoot->priority = rightTree->priority;
        newRoot->right = rightTree->right;
        newRoot->count = rightTree->count;
        newRoot->left = Merge(leftTree, rightTree->left);
        newRoot->sumOfCounts = sumOfCounts(newRoot->left) + sumOfCounts(newRoot->right) + 1 + newRoot->count;
    }
    return newRoot;
}

TNode* RemoveUtil(TNode* root, int key) {
    if (root->key == key) {
        return Merge(root->left, root->right);
    }
    TNode* newRoot = new TNode();

    newRoot->key = root->key;
    newRoot->priority = root->priority;
    newRoot->sumOfCounts = root->sumOfCounts - 1;
    newRoot->count = root->count;
    if (key < root->key) {
        newRoot->right = root->right;
        newRoot->left = RemoveUtil(root->left, key);
    } else {
        newRoot->left = root->left;
        newRoot->right = RemoveUtil(root->right, key);
    }
    return newRoot;
}

TNode* CountNodesWithDecrement(TNode* root, int key) {
    TNode* newRoot = new TNode();

    if (root->key == key) {
        newRoot->left = root->left;
        newRoot->right = root->right;
        newRoot->key = key;
        newRoot->priority = root->priority;
        newRoot->count = root->count - 1;
        newRoot->sumOfCounts = root->sumOfCounts - 1;
        return newRoot;
    }
    newRoot->key = root->key;
    newRoot->priority = root->priority;
    newRoot->sumOfCounts = root->sumOfCounts - 1;
    newRoot->count = root->count;
    if (key < root->key) {
        newRoot->right = root->right;
        newRoot->left = CountNodesWithDecrement(root->left, key);
    } else {
        newRoot->left = root->left;
        newRoot->right = CountNodesWithDecrement(root->right, key);
    }
    return newRoot;
}

TNode* Remove(TNode* root, int key) {
    TNode* node = SearchInTreap(root, key);
    if (!node) {
        return root;
    }
    if (node->count > 0) {
        return CountNodesWithDecrement(root, key);
    }
    return RemoveUtil(root, key);
}

struct TPoint {
    int x;
    int y;
    bool flag;
    TNode* root = nullptr;
    bool operator<(const TPoint& other) const {
        if (x != other.x) {
            return x < other.x;
        } else {
            return flag > other.flag;
        }
    }
};

int GetResult(TNode* root, int y) {
    if (!root) {
        return 0;
    }
    if (root->key <= y) {
        return GetResult(root->right, y);
    } else {
        return sumOfCounts(root->right) + GetResult(root->left, y) + 1 + root->count;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<TPoint> points;
    int n, m;
    std::cin >> n >> m;
    std::vector<int> priorities(2 * n);
    for (int i = 0; i <= 2 * n - 1; ++i) {
        priorities[i] = i;
    }
    std::shuffle(priorities.begin(), priorities.end(), gen);

    for (int i = 0; i < n; ++i) {
        int l, r, h;
        std::cin >> l >> r >> h;
        points.push_back({l, h, true, nullptr});
        points.push_back({r, h, false, nullptr});
    }
    std::sort(points.begin(), points.end());

    points[0].root = Insert(points[0].root, points[0].y, priorities[0]);
    for (int i = 1; i < 2 * n; ++i) {
        if (points[i].flag) {
            points[i].root = Insert(points[i-1].root, points[i].y, priorities[i]);
        } else {
            points[i].root = Remove(points[i-1].root, points[i].y);
        }
    }
    std::vector<TPoint> uniquePoints;
    uniquePoints.push_back(points[2 * n - 1]);
    for (int i = 2 * n - 2; i >= 0; i--) {
        if ((points[i].x != points[i + 1].x) || (points[i].flag != points[i + 1].flag)) {
            uniquePoints.push_back(points[i]);
        }
    }
    std::reverse(uniquePoints.begin(), uniquePoints.end());

    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        int idx = BinarySearch(uniquePoints, x);
        if (idx == -1) {
            std::cout << 0 << '\n';
        } else {
            if (uniquePoints[idx].x == x && !uniquePoints[idx].flag) {
                --idx;
            }
            std::cout << GetResult(uniquePoints[idx].root, y) << '\n';
        }
    }
}
