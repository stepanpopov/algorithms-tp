#pragma once

#include "IGraph.hpp"

#include <vector>
#include <iostream>

class ArcGraph : public IGraph {
public:
    ArcGraph(size_t num);

    ArcGraph(const IGraph &graph);
	
    virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;
    
private:
    struct pair {
        pair(int from, int to) : from(from), to(to) {}

        int from;
        int to;
    };
    int vertices_num = 0;
    std::vector<pair> edges;
};