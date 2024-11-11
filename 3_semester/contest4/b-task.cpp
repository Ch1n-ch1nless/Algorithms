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

bool cmp(Edge &e1, Edge &e2)
{
    return e1.cost < e2.cost;
}

int main()
{
    int vertex_number   = 0;
    int edge_number     = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<Edge> edges(edge_number);

    for (int i = 0; i < edge_number; i++)
    {
        std::cin >> edges[i].begin >> edges[i].end >> edges[i].cost;
        edges[i].begin--;
        edges[i].end--;
    }

    std::sort(edges.begin(), edges.end(), cmp);

    DisjointSetUnion mst(vertex_number);

    int min_cost = 0;

    for (int i = 0; i < edge_number; i++)
    {
        int begin = edges[i].begin;
        int end   = edges[i].end;

        if (mst.getParent(begin) != mst.getParent(end))
        {
            min_cost += edges[i].cost;
            mst.uniteSets(begin, end);
        }
    }

    std::cout << min_cost << '\n';

    return 0;
}