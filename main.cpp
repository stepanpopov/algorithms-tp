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

    template<typename Visit>
    void post_order(Visit visit) {
        if (root == nullptr) {
            return;
        }
        Node *node = root;
        while(true) {
            if (node->left == nullptr && node->right == nullptr) {
                while (true) {   // подъем вверх по дереву
                    visit(node->key);
                    if (node == root) {
                        return;
                    }
                    if (node->parent->left == node) {   // если мы пришли из левого ребенка, то поднимаемся, идем в правый и выходим из while подъема
                        node = node->parent;
                        if (node->right != nullptr) {
                            node = node->right;
                            break;
                        }
                    } else if (node->parent->right == node) {   // если мы пришли из правого ребенка, то поднимаемся и отдаем в visit
                        node = node->parent;
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

private:
    struct Node {
        Node() : key(0) {}

        Node(const int &key) : key(key) {}

        T key;
        Node *parent = nullptr;
        Node *right = nullptr; 
        Node *left = nullptr;
    };

    void delete_nodes() {
        auto delete_visit = [&output](const int &key) {
            output.push_back(key);
        };
        post_order(delete_visit);
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

void test() {
    std::less<int> less;
    BinTree<int, std::less<int>> bin_tree(less);

    bin_tree.insert(8);
    bin_tree.insert(6);
    bin_tree.insert(7);
    // bin_tree.insert(8);
    bin_tree.insert(11);
    bin_tree.insert(10);
    bin_tree.insert(12);

    bin_tree.test();

}

int main() {
    test();


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

// TODO переделать обходы в private 