#pragma once

#include "IGraph.hpp"

#include <vector>
#include <iostream>

class ListGraph : public IGraph {
public:
    ListGraph(size_t num);

    ListGraph(const IGraph &graph);

    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> reverseAdjLists;
};
