#include "ListGraph.hpp"

ListGraph::ListGraph(size_t num) : adjLists(num),
                            reverseAdjLists(num) {}

ListGraph::ListGraph(const IGraph &graph) : ListGraph(graph.VerticesCount()) {

    for (int from = 0; from < adjLists.size(); ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (const int &to : nextVertices) {
            adjLists[from].push_back(to);
            reverseAdjLists[to].push_back(from);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    adjLists[from].push_back(to);
    reverseAdjLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    return adjLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    return reverseAdjLists[vertex];
}
