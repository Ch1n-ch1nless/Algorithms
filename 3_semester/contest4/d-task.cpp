#include <algorithm>
#include <iostream>
#include <vector>

class DisjointSetUnion
{
public:
     DisjointSetUnion(int set_number);
    ~DisjointSetUnion() = default;

    int  getParent(int set_index);
    void uniteSets(int set1, int set2);

private:
    std::vector<int>    parent_;
    std::vector<int>    size_;
};

DisjointSetUnion::DisjointSetUnion(int set_number)
:
    parent_ (set_number),
    size_   (set_number, 1)
{
    for (int i = 0; i < set_number; i++)
    {
        parent_[i] = i;
    }
}

int DisjointSetUnion::getParent(int set_index)
{
    return (set_index == parent_[set_index]) ? 
            set_index                        : 
            parent_[set_index] = getParent(parent_[set_index]);
}

void DisjointSetUnion::uniteSets(int set1, int set2)
{
    set1 = getParent(set1);
    set2 = getParent(set2);

    if (set1 == set2) return;

    if (size_[set1] < size_[set2])
    {
        std::swap(set1, set2);
    }

    size_[set1] += size_[set2];
    parent_[set2] = set1;
}

struct Edge
{
    int begin;
    int end;
    int cost;
};

bool cmp(Edge& edge1, Edge& edge2)
{
    return edge1.cost > edge2.cost;
}

class Graph
{
public:
     Graph(int vertex_number);
    ~Graph() = default;

    void addEdge(const Edge& edge);

    std::vector<Edge>& operator[](int vertex);
    const std::vector<Edge>& operator[](int vertex) const;

    int getVertexNumber() const;

private:
    std::vector<std::vector<Edge>> adjacency_list_;
};

Graph::Graph(int vertex_number) : adjacency_list_(vertex_number) {}

void Graph::addEdge(const Edge& edge)
{
    adjacency_list_[edge.begin].push_back(edge);
}

std::vector<Edge>& Graph::operator[](int vertex)
{
    return adjacency_list_[vertex];
}

const std::vector<Edge>& Graph::operator[](int vertex) const
{
    return adjacency_list_[vertex];
}

int Graph::getVertexNumber() const
{
    return static_cast<int>(adjacency_list_.size());
}

class MinimalSpanningTree
{
public:
    MinimalSpanningTree(const Graph& src);

    ~MinimalSpanningTree() = default;

          Graph& getGraph();
    const Graph& getGraph() const;

private:
    Graph   tree_;

    void build(const Graph& src, Graph& mst);
};

MinimalSpanningTree::MinimalSpanningTree(const Graph& src) 
: 
    tree_(src.getVertexNumber()) 
{
    build(src, tree_);
}

void MinimalSpanningTree::build(const Graph& src, Graph& tree)
{
    std::vector<Edge> edge_list = {};

    for (int i = 0; i < src.getVertexNumber(); ++i)
    {
        for (const Edge& new_edge : src[i])
        {
            edge_list.push_back(new_edge);
        }
    }

    std::sort(edge_list.begin(), edge_list.end(), cmp);

    DisjointSetUnion dsu(src.getVertexNumber());

    for (std::size_t i = 0; i < edge_list.size(); ++i)
    {
        int begin = edge_list[i].begin;
        int end   = edge_list[i].end;

        if (dsu.getParent(begin) != dsu.getParent(end))
        {
            tree[begin].push_back(edge_list[i]);
            tree[end].push_back(Edge{edge_list[i].end, edge_list[i].begin, edge_list[i].cost});
            dsu.uniteSets(begin, end);
        }
    }
}

Graph& MinimalSpanningTree::getGraph()
{
    return tree_;
}

const Graph& MinimalSpanningTree::getGraph() const
{
    return tree_;
}

class SparseTable
{

};

int main()
{


    return 0;
}

