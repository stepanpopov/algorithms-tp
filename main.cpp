#include <iostream>
#include <queue>
#include <vector>

template<class T, class Comparator>
class BinTree {
public:

    BinTree(Comparator cmp) : cmp(cmp) {}

    ~BinTree() {
        nodes_delete(root);
    }

    void insert(const T& key) {
        if (root == nullptr) {
            root = new Node(key);
            return;
        }

        Node *node = root;
        while(true) {
            if (cmp(key,node->key)) {
                if (node->left == nullptr) {
                    node->left = new Node(key);
                    return;
                }
                node = node->left;
            } else {
                if (node->right == nullptr) {
                    node->right = new Node(key);
                    return;
                }
                node = node->right;
            }
        }
    }

    template<typename Visit>
    void level_order(Visit visit) {
        if (root == nullptr) {
            return;
        }
        std::queue<Node *> q;
        q.push(root);
        while(!q.empty()) {
            Node *node = q.front();
            q.pop();
            visit(node->key);
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }

private:
    struct Node {
        Node() : key(0) {}

        Node(const int &key) : key(key) {}

        T key;
        Node *right = nullptr; 
        Node *left = nullptr;
    };

    void nodes_delete(Node *n) {
        if (n == nullptr) {
            return;
        }
        nodes_delete(n->left);
        nodes_delete(n->right);

        delete n;
    }

    Comparator cmp;
    Node *root = nullptr;
};

void bfs(const std::vector<int> &input, std::vector<int> &output) {
    std::less<int> less;
    BinTree<int, std::less<int>> bin_tree(less);
    for(int el : input) {
        bin_tree.insert(el);
    }

    auto lmd = [&output](const int &key) {
        output.push_back(key);
    };
    bin_tree.level_order(lmd);
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> input;
    for(int i = 0; i < n; ++i) {
        int temp;
        std::cin >> temp;
        input.push_back(temp);
    }

    std::vector<int> output;
    bfs(input, output);
    for (int &el : output) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

// TODO delete remove?