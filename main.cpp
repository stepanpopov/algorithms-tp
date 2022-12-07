#include <iostream>
#include <string>
#include <sstream>
#include <set>

struct Node {
    Node() : key(0) {}

    Node(const int &key) : key(key) {}

    int key;
    Node *right = nullptr; 
    Node *left = nullptr;
};

class BTree {
public:
    BTree() = default;
    ~BTree() {
        delete_nodes(root);
    }

    void insert(const int& key) {
        node_insert(root, key);
    }

    int min_distance() {
        int min_dist = 0;
        min_distance_helper(root, 1, min_dist);
        return min_dist;
    }

private:
    void node_insert(Node *&n, const int& key) {
        if (n == nullptr) {
            n = new Node(key);
        }

        if (key < n->key) {
            node_insert(n->left, key);
        } else if (n->key < key) {
           node_insert(n->right, key);
        }
    }

    void min_distance_helper(Node *n, int distance, int &min_dist) {
        if (n == nullptr) {
            return;
        }

        if (n->left == nullptr && n->right == nullptr) {
            min_dist = (min_dist == 0 ? distance : std::min(min_dist, distance));
        }

        min_distance_helper(n->left, distance + 1, min_dist);
        min_distance_helper(n->right, distance + 1, min_dist);
    }

    void delete_nodes(Node *n) {
        if (n == nullptr) {
            return;
        }
        delete_nodes(n->left);
        delete_nodes(n->right);

        delete n;
    }

    Node *root = nullptr;
};

int main() {
    std::string s;
    std::getline(std::cin, s);
    std::stringstream ss(s);

    BTree tree;

    int el;
    while(ss >> el) {
        tree.insert(el);
    }
    std::cout << tree.min_distance();

}

