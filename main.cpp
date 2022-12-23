#include <vector>
#include <iostream>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}
	
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
public:
    ListGraph(size_t num) : adjList(num),
                            reverseAdjList(num) {}

    ListGraph(const IGraph &graph) : adjList(graph.VerticesCount()),
                                     reverseAdjList(graph.VerticesCount()) {
        for (size_t from = 0; from < adjList.size(); ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (int &to : nextVertices) {
                adjList[from].push_back(to);
                reverseAdjList[to].push_back(from);
            }
        }
    }

    virtual void AddEdge(int from, int to) override {
        adjList[from].push_back(to);
        reverseAdjList[to].push_back(from);
    }

    virtual int VerticesCount() const override {
        return adjList.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        return adjList[vertex];
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        return reverseAdjList[vertex];
    }


private:
    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<int>> reverseAdjList;
};

struct edge_t {
    int from;
    int to;
};

void fill(IGraph &graph, const std::vector<edge_t> &edges) {
    for (const edge_t &edge : edges) {
        graph.AddEdge(edge.from, edge.to);
        graph.AddEdge(edge.to, edge.from);
    }
}

int shortestWaysCount(const IGraph &graph, int from, int to) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> shortestWays(graph.VerticesCount(), 0);
    std::vector<int> level(graph.VerticesCount(), 0);
    std::queue<int> q;
    
    q.push(from);
    visited[from] = true;
    shortestWays[from] = 1;
    while(!q.empty()) {
        int cur = q.front();
        q.pop();

        std::vector<int> nextVertices = graph.GetNextVertices(cur);
        for(int &nextVertice: nextVertices) {
            if (!visited[nextVertice]) {
                q.push(nextVertice);
                level[nextVertice] = level[cur] + 1;
                visited[nextVertice] = true;
            }
            
            if (level[nextVertice] > level[cur]) {
                shortestWays[nextVertice] += shortestWays[cur];
            }
        }
        
    }
    return shortestWays[to];
}

int main() {
    int v, n;
    std::cin >> v >> n;
    std::vector<edge_t> edges(n);
    for(int i = 0; i < n; ++i) {
        std::cin >> edges[i].from;
        std::cin >> edges[i].to;  
    }
    int from, to;
    std::cin >> from >> to;

    ListGraph *listG = new ListGraph(v);
    fill(*listG, edges);

    std::cout << shortestWaysCount(*listG, from, to) << std::endl;

    return 0;
}
