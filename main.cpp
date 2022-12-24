#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>
#include <set>

#include "SetGraph.hpp"
#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"

#define N 1000
#define M 10000

struct edge_t {
    int from;
    int to;
};

void fill(IGraph &graph, const std::vector<edge_t> &edges) {
    for (const edge_t &edge : edges) {
        graph.AddEdge(edge.from, edge.to);
    }
}

std::set<int> get_next_ordered(const IGraph &graph, int vertex) {
    std::vector<int> v = graph.GetNextVertices(vertex);
    return std::set(v.begin(), v.end());
}

std::set<int> get_prev_ordered(const IGraph &graph, int vertex) {
    std::vector<int> v = graph.GetPrevVertices(vertex);
    return std::set(v.begin(), v.end());
}

void test() {
    std::vector<edge_t> edges;
    std::srand(std::time(nullptr));
    for (int i = 0; i < M; ++i) {
        int from = rand() % N;
        int to = rand() % N;
        edges.push_back({from, to});
    }

    /*ListGraph *g1 = new ListGraph(N);
    MatrixGraph *g2 = new MatrixGraph(N);
    ArcGraph *g3 = new ArcGraph(N);
    SetGraph *g4 = new SetGraph(N);

    fill(*g1, edges);
    fill(*g2, edges);
    fill(*g3, edges);
    fill(*g4, edges);*/

    ListGraph *g1 = new ListGraph(N);
    fill(*g1, edges);
    MatrixGraph *g2 = new MatrixGraph(*g1);
    ArcGraph *g3 = new ArcGraph(*g2);
    SetGraph *g4 = new SetGraph(*g3);


    std::cout << "NEXT\n"; 
    for (int i = 0; i < N; ++i) {
        auto s1 = get_next_ordered(*g1, i);
        auto s2 = get_next_ordered(*g2, i);
        auto s3 = get_next_ordered(*g3, i);
        auto s4 = get_next_ordered(*g4, i);

        assert(s1 == s2);
        assert(s3 == s4);
        assert(s1 == s3);
    }

    std::cout << "PREV\n";
    for (int i = 0; i < N; ++i) {
        auto s1 = get_prev_ordered(*g1, i);
        auto s2 = get_prev_ordered(*g2, i);
        auto s3 = get_prev_ordered(*g3, i);
        auto s4 = get_prev_ordered(*g4, i);

        assert(s1 == s2);
        assert(s3 == s4);
        assert(s1 == s3);
    }

    delete g1;
    delete g2;
    delete g3;
    delete g4;
}

int main() {
    test();

    std::cout << "SUCCESS\n";
    return 0;
}
