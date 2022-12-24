#include "SetGraph.hpp"

SetGraph::SetGraph(size_t num) : sets(num) {}

SetGraph::SetGraph(const IGraph &graph) : SetGraph(graph.VerticesCount()) {
        for (int from = 0; from < sets.size(); ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (const int &to : nextVertices) {
                sets[from].insert(to);
            }
        }
    }

void SetGraph::AddEdge(int from, int to) {
    sets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return sets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    return std::vector<int>(sets[vertex].begin(), sets[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;
    for (int i = 0; i < sets.size(); ++i) {
        if (sets[i].find(vertex) != sets[i].end()) {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}

bool SetGraph::HasEdge(int to, int from) const {
    return (sets[to].find(from) != sets[to].end());
}
