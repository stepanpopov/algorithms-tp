#include <iostream>
#include <queue>
#include <vector>

template<class T, class Comparator = std::less<T>>
class BinTree {
public:

    BinTree(Comparator cmp = Comparator()) : cmp(cmp) {}

    ~BinTree() {
        delete_nodes();
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
                    node->left->parent = node;
                    return;
                }
                node = node->left;
            } else {
                if (node->right == nullptr) {
                    node->right = new Node(key);
                    node->right->parent = node;
                    return;
                }
                node = node->right;
            }
        }
    }

    template<typename Visit>
    void level_order(Visit visit) {
        auto lmd_by_key = [&visit](Node *n) {
            visit(n->key);
        };
        node_level_order(lmd_by_key);
    }

    template<typename Visit>
    void post_order(Visit visit) {
        auto lmd_by_key = [&visit](Node *n) {
            visit(n->key);
        };
        node_post_order(lmd_by_key);
    }

    
private:
    struct Node {
        Node() : key(0) {}

        Node(const int &key) : key(key) {}

        T key;
        Node *parent = nullptr;
        Node *right = nullptr; 
        Node *left = nullptr;
    };

    template<typename Visit>
    void node_level_order(Visit visit) {
        if (root == nullptr) {
            return;
        }
        std::queue<Node *> q;
        q.push(root);
        while(!q.empty()) {
            Node *node = q.front();
            q.pop();
            visit(node);
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }

    template<typename Visit>
    void node_post_order(Visit visit) {
        if (root == nullptr) {
            return;
        }
        Node *node = root;
        while(true) {
            if (node->left == nullptr && node->right == nullptr) {
                while (true) {   // подъем вверх по дереву
                    Node *parent = node->parent;
                    visit(node);
                    if (node == root) {
                        return;
                    }
                    if (parent->left == node) {   // если мы пришли из левого ребенка, то поднимаемся, идем в правый и выходим из while подъема
                        node = parent;
                        if (node->right != nullptr) {
                            node = node->right;
                            break;
                        }
                    } else if (parent->right == node) {   // если мы пришли из правого ребенка, то поднимаемся и отдаем в visit
                        node = parent;
                    }
                }
            }

            while(node->left != nullptr) {
                node = node->left;
            }
            if (node->right != nullptr) {
                node = node->right;
            }
        }
    }

    void delete_nodes() {
        auto delete_visit = [](Node *node) {
            delete node;
        };
        node_post_order(delete_visit);
    }

    Comparator cmp;
    Node *root = nullptr;
};

void bfs_to_output(const std::vector<int> &input, std::vector<int> &output) {
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
    bfs_to_output(input, output);
    for (int &el : output) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}
