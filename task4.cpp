#include <iostream>
#include <vector>  // test

struct Node {
    Node() : key(0) {}

    Node(const int &key) : key(key) {}  

    int key;
    size_t height = 1;
    Node *right = nullptr;
    Node *left = nullptr;
};

class AVLTree {
public:
    AVLTree() = default;

    ~AVLTree() {
        node_visit_and_delete(root);
    }

    void insert(const int &key) {
        root = node_insert(root, key);
    }

    void remove(const int &key) {
        root = node_remove(root, key);
    }

    bool exists(const int &key) {
        return (node_find(root, key) != nullptr);
    }

    int next(const int &key) {
        /*Node *next_n = nullptr;
        bool found = false;
        node_next(root, key, found, next_n);
        if (next_n == nullptr) {
            return -1;
        } else {
            return next_n->key;
        }*/

        return node_next2(root, key, -1);
    }

    int prev(const int &key) {
        /*Node *prev_n = nullptr;
        bool found = false;
        node_prev(root, key, found, prev_n);
        if (prev_n == nullptr) {
            return -1;
        } else {
            return prev_n->key;
        }*/

        return node_prev2(root, key, -1);
    }

private:
    void node_visit_and_delete(Node *n) {
        if (n == nullptr) {
            return;
        }
        node_visit_and_delete(n->left);
        node_visit_and_delete(n->right);

        delete n;
    }

    int node_next2(Node *n, const int &key, const int &candidate) {
        if (n == nullptr) {
            return candidate;
        }

        if (key < n->key) {
            return node_next2(n->left, key, n->key);
        } else {
            return node_next2(n->right, key, candidate);
        }
    }

    void node_next(Node *n, const int &key, bool &next_set, Node *&next_n) const {
        if (n == nullptr) {
            return;
        }
        node_next(n->left, key, next_set, next_n);
        
        if (n->key > key) {
            if (!next_set) {
                next_n = n;
                next_set = true;
            } 
        }

        node_next(n->right, key, next_set, next_n);
    }

    int node_prev2(Node *n, const int &key, const int &candidate) {
        if (n == nullptr) {
            return candidate;
        }

        if (key <= n->key) {
            return node_prev2(n->left, key, candidate);
        } else {
            return node_prev2(n->right, key, n->key);
        }
    }

    void node_prev(Node *n, const int &key, bool &prev_set, Node *&prev_n) const {
        if (n == nullptr) {
            return;
        }
        node_prev(n->right, key, prev_set, prev_n);
        
        if (n->key < key) {
            if (!prev_set) {
                prev_n = n;
                prev_set = true;
            } 
        }

        node_prev(n->left, key, prev_set, prev_n);
    }

    Node *node_find(Node *n, const int &key) const {  // exists
        if (n == nullptr) {
            return nullptr;
        }

        if (key < n->key) {
            return node_find(n->left, key);
        } else if (n->key < key) {
            return node_find(n->right, key);
        } else {
            return n;
        }
    }

    // INSERT

    Node *node_insert(Node *n, const int &key) {
        if (n == nullptr) {
            return new Node(key);
        }

        if (key < n->key) {
            n->left = node_insert(n->left, key);
        } else if (n->key < key) {
            n->right = node_insert(n->right, key);
        }

        return node_balance(n);
    }

    // REMOVE

    Node *node_find_min(Node *n) const {
        if (n->left == nullptr) {
            return n;
        }
        return node_find_min(n->left);
    }

    Node *node_without_min(Node *n) {
        if (n->left == nullptr) {
            return n->right;
        }
        n->left = node_without_min(n->left);
        return node_balance(n);
    }

    Node *node_remove(Node *n, const int &key) {
        if (n == nullptr) {
            return nullptr;
        }
        if (key < n->key) {
            n->left = node_remove(n->left, key);
        } else if (n->key < key) {
            n->right = node_remove(n->right, key);
        } else {
            Node *node_left = n->left;
            Node *node_right = n->right;
            delete n;

            if (node_right == nullptr) {
                return node_left;
            }

            Node *node_min = node_find_min(node_right);
            node_min->right = node_without_min(node_right);
            node_min->left = node_left;

            return node_balance(node_min);
        }

        return node_balance(n);
    }

    // BALANCE

    int node_balance_factor(Node *n) const {
        return node_height(n->right) - node_height(n->left);
    }

    void node_fix_height(Node *n) {
        size_t height_left = node_height(n->left);
        size_t height_right = node_height(n->right);

        n->height = std::max(height_left, height_right) + 1;
    }

    size_t node_height(Node *n) const {
        if (n != nullptr) {
            return n->height;
        } else {
            return 0;
        }
    }

    Node *node_rotate_right(Node *n) {
        Node *q = n->left;
        n->left = q->right;
        q->right = n;
        node_fix_height(n);
        node_fix_height(q);
        return q;
    }

    Node *node_rotate_left(Node *n) {
        Node *p = n->right;
        n->right = p->left;
        p->left = n;
        node_fix_height(n);
        node_fix_height(p);
        return p;
    }

    Node *node_balance(Node *n) {
        node_fix_height(n);
        if (node_balance_factor(n) == 2 && node_balance_factor(n->right) < 0) {
            n->right = node_rotate_right(n->right);
            return node_rotate_left(n);
        } else if (node_balance_factor(n) == 2 && node_balance_factor(n->right) >= 0) {
            return node_rotate_left(n);
        } else if (node_balance_factor(n) == -2 && node_balance_factor(n->left) > 0) {
            n->left = node_rotate_left(n->left);
            return node_rotate_right(n);
        } else if (node_balance_factor(n) == -2 && node_balance_factor(n->left) <= 0) {
            return node_rotate_right(n);
        }
        return n;
    }

    Node *root = nullptr;
};

int main() {
    AVLTree avl;

    std::vector<int> vec{1, 2, 3, 5, 8, 13, 14};
    for(int i = 0; i < vec.size(); ++i) {
        avl.insert(vec[i]);
    }

    for(auto i : vec) {
        std::cout << i << " " << avl.prev(i) << std::endl;
    }
    std::cout << 16 << " " << avl.prev(16) << std::endl;

    return 0;
}
