#include "MatrixGraph.hpp"

MatrixGraph::MatrixGraph(size_t num) : vertices_num(num), matrix(num * num, 0) {}

MatrixGraph::MatrixGraph(const IGraph &graph) : MatrixGraph(graph.VerticesCount()) {

    for (int from = 0; from < vertices_num; ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (const int &to : nextVertices) {
            matrix[from * vertices_num + to] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix[from * vertices_num + to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return vertices_num;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;
    for (int i = 0; i < vertices_num; ++i) {
        if (matrix[vertex * vertices_num + i] == 1) {
            nextVertices.push_back(i);
        }
    }

    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;
    for (int i = 0; i < vertices_num; ++i) {
        if (matrix[i * vertices_num + vertex] == 1) {
            prevVertices.push_back(i);
        }
    }

    return prevVertices;
}

bool MatrixGraph::HasEdge(int from, int to) const {
    return (matrix[from * vertices_num + to] == 1);
}
