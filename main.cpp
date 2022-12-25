#include <vector>
#include <iostream>
#include <set>

struct Node {
    int ver;
    int weight;
};

struct IGraph {
    virtual ~IGraph() {}
	
    virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<Node> GetNextVertices(int vertex) const = 0;
    virtual std::vector<Node> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
public:
    ListGraph(size_t num) : adjLists(num),
                            reverseAdjLists(num) {}

    virtual void AddEdge(int from, int to, int weight) override {
        adjLists[from].push_back({to, weight});
        reverseAdjLists[to].push_back({from, weight});
    }

    virtual int VerticesCount() const override {
        return adjLists.size();
    }

    virtual std::vector<Node> GetNextVertices(int vertex) const override {
        return adjLists[vertex];
    }

    virtual std::vector<Node> GetPrevVertices(int vertex) const override {
        return reverseAdjLists[vertex];
    }

private:
    std::vector<std::vector<Node>> adjLists;
    std::vector<std::vector<Node>> reverseAdjLists;
};


int Dijkstra(const IGraph &graph, int from, int to) {
    std::vector<int> distance(graph.VerticesCount(), -1);
    distance[from] = 0;

    auto cmp = [&distance](int ver1, int ver2) {
        return distance[ver1] < distance[ver2];
    };

    std::set<int, decltype(cmp)> s(cmp);
    s.insert(from);

    while(!s.empty()) {
        int cur = *s.begin();
        s.erase(s.begin());

        std::vector<Node> nextVertices = graph.GetNextVertices(cur);
        for (const Node &next : nextVertices) {
            if (distance[next.ver] == -1) {
                distance[next.ver] = distance[cur] + next.weight;
                s.insert(next.ver);
            } else if (distance[next.ver] > distance[cur] + next.weight) {
                s.erase(next.ver);
                distance[next.ver] = distance[cur] + next.weight;
                s.insert(next.ver);
            }
        }
    }

    return distance[to];
}

struct edge_t {
    int from;
    int to;
    int weight;
};


void fill(IGraph &graph, const std::vector<edge_t> &edges) {
    for (const edge_t &edge : edges) {
        graph.AddEdge(edge.from, edge.to, edge.weight);
        graph.AddEdge(edge.to, edge.from, edge.weight);
    }
}


int main() {
    int N, M;
    std::cin >> N >> M;

    std::vector<edge_t> input(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> input[i].from >> input[i].to >> input[i].weight;
    }
    int from, to;
    std::cin >> from >> to;

    ListGraph *graph = new ListGraph(N);
    fill(*graph, input);
    std::cout << Dijkstra(*graph, from, to) << std::endl;

    delete graph;
    return 0;
}
