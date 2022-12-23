#include <vector>
#include <iostream>

struct IGraph {
    virtual ~IGraph() {}
	
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : IGraph {
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

int main() {


    return 0;
}