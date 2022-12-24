#include "ArcGraph.hpp"


ArcGraph::ArcGraph(size_t num) : vertices_num(num) {
        edges.reserve(num);
}

ArcGraph::ArcGraph(const IGraph &graph) : ArcGraph(graph.VerticesCount()) {
    for (int from = 0; from < vertices_num; ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (const int &to : nextVertices) {
            edges.emplace_back(from, to);
        }
    }
}
void ArcGraph::AddEdge(int from, int to) {
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return vertices_num;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;
    for (const pair &p : edges) {
        if (p.from == vertex) {
            nextVertices.push_back(p.to);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;
    for (const pair &p : edges) {
        if (p.to == vertex) {
            prevVertices.push_back(p.from);
        }
    }
    return prevVertices;
}
