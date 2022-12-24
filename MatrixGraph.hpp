#pragma once

#include "IGraph.hpp"

#include <vector>
#include <iostream>


class MatrixGraph : public IGraph {
public:
    MatrixGraph(size_t num);

    MatrixGraph(const IGraph &graph);

    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

    bool HasEdge(int from, int to) const;

private:
    int vertices_num = 0;
    std::vector<char> matrix;
};
