#include <vector>
#include <iostream>
#include <unordered_set>

struct IGraph {
    virtual ~IGraph() {}
	
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};



// LIST GRAPH
class ListGraph : IGraph {
public:
    ListGraph(size_t num) : adjLists(num),
                            reverseAdjLists(num) {}

    ListGraph(const IGraph &graph) : ListGraph(graph.VerticesCount()) {

        for (int from = 0; from < adjLists.size(); ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (const int &to : nextVertices) {
                adjLists[from].push_back(to);
                reverseAdjLists[to].push_back(from);
            }
        }
    }

    virtual void AddEdge(int from, int to) override {
        adjLists[from].push_back(to);
        reverseAdjLists[to].push_back(from);
    }

    virtual int VerticesCount() const override {
        return adjLists.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        return adjLists[vertex];
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        return reverseAdjLists[vertex];
    }


private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> reverseAdjLists;
};



// MATRIX GRAPH
class MatrixGraph : public IGraph {
public:
    MatrixGraph(size_t num) : vertices_num(num), matrix(num * num, 0) {}

    MatrixGraph(const IGraph &graph) : MatrixGraph(graph.VerticesCount()) {

        for (int from = 0; from < vertices_num; ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (const int &to : nextVertices) {
                matrix[from * vertices_num + to] = 1;
            }
        }
    }

    virtual void AddEdge(int from, int to) override {
        matrix[from * vertices_num + to] = 1;
    }

    virtual int VerticesCount() const override {
        return vertices_num;
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> nextVertices;
        for (int i = 0; i < vertices_num; ++i) {
            if (matrix[vertex * vertices_num + i] == 1) {
                nextVertices.push_back(i);
            }
        }

        return nextVertices;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> prevVertices;
        for (int i = 0; i < vertices_num; ++i) {
            if (matrix[i * vertices_num + vertex] == 1) {
                prevVertices.push_back(i);
            }
        }

        return prevVertices;
    }

private:
    int vertices_num = 0;
    std::vector<char> matrix;
};



// SET GRAPH
class SetGraph : public IGraph {
public:
    SetGraph(size_t num) : sets(num) {}

    SetGraph(const IGraph &graph) : SetGraph(graph.VerticesCount()) {
        for (int from = 0; from < sets.size(); ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (const int &to : nextVertices) {
                sets[from].insert(to);
            }
        }
    }
    
    virtual void AddEdge(int from, int to) override {
        sets[from].insert(to);
    }

	virtual int VerticesCount() const override {
        return sets.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        return std::vector<int>(sets[vertex].begin(), sets[vertex].end());
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> prevVertices;
        for (int i = 0; i < sets.size(); ++i) {
            if (sets[i].find(vertex) == sets[i].end()) {
                prevVertices.push_back(i);
            }
        }
        return prevVertices;
    }

    bool HasEdge(int to, int from) const {
        return (sets[to].find(from) != sets[to].end());
    }

private:
    std::vector<std::unordered_set<int>> sets;
};



// ARC GRAPH
class ArcGraph : public IGraph {
public:
    ArcGraph(size_t num) : vertices_num(num) {
        edges.reserve(num);
    }

    ArcGraph(const IGraph &graph) : ArcGraph(graph.VerticesCount()) {
        for (int from = 0; from < vertices_num; ++from) {
            std::vector<int> nextVertices = graph.GetNextVertices(from);
            for (const int &to : nextVertices) {
                edges.emplace_back(from, to);
            }
        }
    }
	
    virtual void AddEdge(int from, int to) override {
        edges.emplace_back(from, to);
    }

	virtual int VerticesCount() const override {
        return vertices_num;
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> nextVertices;
        for (const pair &p : edges) {
            if (p.from == vertex) {
                nextVertices.push_back(p.to);
            }
        }
        return nextVertices;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> prevVertices;
        for (const pair &p : edges) {
            if (p.to == vertex) {
                prevVertices.push_back(p.from);
            }
        }
        return prevVertices;
    }

private:
    struct pair {
        pair(int from, int to) : from(from), to(to) {}

        int from;
        int to;
    };
    int vertices_num = 0;
    std::vector<pair> edges;
};


int main() {


    return 0;
}