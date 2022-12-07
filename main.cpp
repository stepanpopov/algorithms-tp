#include <iostream>
#include <vector>  // input

template<class T>
struct Node {
    Node() : key(T()) {}

    Node(const T &key) : key(key) {}

    T key;
    size_t height = 1;
    size_t count = 1;
    Node *right = nullptr;
    Node *left = nullptr;
};

template<class T, typename Comparator = std::less<T>>
class AVLTree {
public:
    AVLTree(Comparator cmp = Comparator()) : cmp(cmp) {}
    
    AVLTree(const AVLTree<T> &copy_tree) : AVLTree(copy_tree.cmp) {
        node_visit_and_copy(root, copy_tree.root);
    } 

    AVLTree<T> &operator=(const AVLTree<T> &copy_tree) {
        if (this == &copy_tree) {
            return *this;
        }

        node_visit_and_delete(root);
        root = nullptr;
        node_visit_and_copy(root, copy_tree.root);
        cmp = copy_tree.cmp;

        return *this;
    }   

    ~AVLTree() {
        node_visit_and_delete(root);
    }

    void insert(const T &key) {
        root = node_insert(root, key);
    }

    void remove(const T &key) {
        root = node_remove(root, key);
    }

    bool exists(const T &key) const {
        return (node_find(root, key) != nullptr);
    }

    T k_th_statistics(const size_t pos) const {
        // if (pos > node_count(root) - 1) return baaad;
        return node_k_th_statistics(root, pos)->key;
    }


private:
    // COPY

    static void node_visit_and_copy(Node<T> *&n, Node<T> *copy_node) {
        if (copy_node == nullptr) {
            return;
        }
        n = new Node<T>(copy_node->key);
        n->count = copy_node->count;

        node_visit_and_copy(n->left, copy_node->left);
        node_visit_and_copy(n->right, copy_node->right);
    }

    // DESTRUCT

    static void node_visit_and_delete(Node<T> *n) {
        if (n == nullptr) {
            return;
        }
        node_visit_and_delete(n->left);
        node_visit_and_delete(n->right);

        delete n;
    }

    // K_TH_STATISTICS

    static Node<T> *node_k_th_statistics(Node<T> *n, const size_t pos) {
        if (n == nullptr) {
            return nullptr;
        }

        size_t node_left_count = node_count(n->left);
        if (pos == node_left_count) {
            return n;
        } else if (pos < node_left_count) {
            return node_k_th_statistics(n->left, pos);
        } else {
            return node_k_th_statistics(n->right, pos - node_left_count - 1);
        }
    }


    // FIND

    Node<T> *node_find(Node<T> *n, const T &key) const { 
        if (n == nullptr) {
            return nullptr;
        }

        if (cmp(key, n->key)) {
            return node_find(n->left, key);
        } else if (cmp(n->key, key)) {
            return node_find(n->right, key);
        } else {
            return n;
        }
    }

    // INSERT

    Node<T> *node_insert(Node<T> *n, const T &key) {
        if (n == nullptr) {
            return new Node<T>(key);
        }

        if (cmp(key, n->key)) {
            n->left = node_insert(n->left, key);
        } else if (cmp(n->key, key)) {
            n->right = node_insert(n->right, key);
        }

        return node_balance(n);
    }

    // REMOVE

    static Node<T> *node_find_min(Node<T> *n) {
        if (n->left == nullptr) {
            return n;
        }
        return node_find_min(n->left);
    }

    static Node<T> *node_without_min(Node<T> *n) {
        if (n->left == nullptr) {
            return n->right;
        }
        n->left = node_without_min(n->left);
        return node_balance(n);
    }

    Node<T> *node_remove(Node<T> *n, const int &key) {
        if (n == nullptr) {
            return nullptr;
        }
        if (cmp(key, n->key)) {
            n->left = node_remove(n->left, key);
        } else if (cmp(n->key, key)) {
            n->right = node_remove(n->right, key);
        } else {
            Node<T> *node_left = n->left;
            Node<T> *node_right = n->right;
            delete n;

            if (node_right == nullptr) {
                return node_left;
            }

            Node<T> *node_min = node_find_min(node_right);
            node_min->right = node_without_min(node_right);
            node_min->left = node_left;

            return node_balance(node_min);
        }

        return node_balance(n);
    }

    // BALANCE

    static int node_balance_factor(Node<T> *n) {
        return node_height(n->right) - node_height(n->left);
    }

    static void node_fix_height(Node<T> *n) {
        size_t height_left = node_height(n->left);
        size_t height_right = node_height(n->right);

        n->height = std::max(height_left, height_right) + 1;
    }

    static void node_fix_count(Node<T> *n) {
        size_t count_left = node_count(n->left);
        size_t count_right = node_count(n->right);

        n->count = count_left + count_right + 1;
    }

    static size_t node_height(Node<T> *n) {
        if (n != nullptr) {
            return n->height;
        } else {
            return 0;
        }
    }

    static size_t node_count(Node<T> *n) {
        if (n != nullptr) {
            return n->count;
        } else {
            return 0;
        }
    }

    static Node<T> *node_rotate_right(Node<T> *n) {
        Node<T> *q = n->left;
        n->left = q->right;
        q->right = n;
        node_fix_height(n);
        node_fix_count(n);
        node_fix_height(q);
        node_fix_count(q);
        return q;
    }

    static Node<T> *node_rotate_left(Node<T> *n) {
        Node<T> *p = n->right;
        n->right = p->left;
        p->left = n;
        node_fix_height(n);
        node_fix_count(n);
        node_fix_height(p);
        node_fix_count(p);
        return p;
    }

    static Node<T> *node_balance(Node<T> *n) {
        node_fix_height(n);
        node_fix_count(n);
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

    Comparator cmp;
    Node<T> *root = nullptr;
};


struct request_t {
    int key;
    bool remove;
    size_t pos;
};

int request(AVLTree<int> &tree, const request_t &req) {
    if (req.remove) {
        tree.remove(req.key);
    } else {
        tree.insert(req.key);
    }
    return tree.k_th_statistics(req.pos);
}

void tree_operations(const std::vector<request_t> &input, std::vector<int> &output) {
    std::less<int> less;
    AVLTree<int, std::less<int>> tree(less);

    for(const request_t &req : input) {
        output.push_back(request(tree, req));
    }
}

int main() {
    size_t n;
    std::cin >> n;
    std::vector<request_t> input;
    for(size_t i = 0; i < n; ++i) {
        int key;
        size_t pos;
        bool remove;
        std::cin >> key >> pos;
        if (key < 0) {
            remove = true;
            key = std::abs(key);
        } else {
            remove = false;
        }
        input.push_back({key, remove, pos});
    }

    std::vector<int> res;
    tree_operations(input, res);
    for(int i : res) {
        std::cout << i << std::endl;
    }

    return 0;
}
