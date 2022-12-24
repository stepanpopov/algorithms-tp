#pragma once

#include "IGraph.hpp"

#include <vector>
#include <unordered_set>
#include <iostream>

class SetGraph : public IGraph {
public:
    SetGraph(size_t num);

    SetGraph(const IGraph &graph);
    
    virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

    bool HasEdge(int to, int from) const;

private:
    std::vector<std::unordered_set<int>> sets;
};
