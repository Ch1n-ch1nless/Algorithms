#include <algorithm>
#include <iostream>
#include <vector>

class Graph
{
public:
    Graph(size_t vertex_number);

    void addEdge(size_t start, size_t finish, long long capacity);
    void updateFlow(size_t start, size_t finish, long long delta);
    long long getCapacity(size_t start, size_t finish) const;
    size_t getVertexNum() const;

private:
    std::vector<std::vector<long long>> capacity_;
    std::vector<std::vector<long long>> flow_;
};

Graph::Graph(size_t vertex_number)
:
    capacity_(vertex_number, std::vector<long long>(vertex_number, 0ll)),
    flow_(vertex_number, std::vector<long long>(vertex_number, 0ll))
{
}

void Graph::addEdge(size_t start, size_t finish, long long capacity)
{
    capacity_[start][finish] = capacity;
    flow_[start][finish]     = 0ll;
}

void Graph::updateFlow(size_t start, size_t finish, long long delta)
{
    flow_[start][finish] += delta;
    flow_[finish][start] -= delta;
}

long long Graph::getCapacity(size_t start, size_t finish) const
{
    return capacity_[start][finish] - flow_[start][finish];
}

size_t Graph::getVertexNum() const
{
    return capacity_.size();
}

const long long INF = __INT_MAX__;

long long depthFirstSearch(Graph       &graph,
                           std::vector<bool> &used,
                           size_t start, size_t sink,
                           long long base_capacity   )
{
    if (start == sink) return base_capacity;

    used[start] = true;

    for (size_t next = 0; next < graph.getVertexNum(); ++next)
    {
        if (!used[next] && graph.getCapacity(start, next) > 0)
        {
            long long next_capacity = std::min(base_capacity, 
                                               graph.getCapacity(start, next));

            long long next_flow = depthFirstSearch(graph, used, next, sink, next_capacity);

            if (next_flow > 0)
            {
                graph.updateFlow(start, next, next_flow);
                return next_flow;
            }
        }
    }
    return 0;
}

long long findMaximalFlow(Graph &graph, size_t source, size_t sink)
{
    std::vector<bool> used(graph.getVertexNum());

    long long max_flow = 0;
    long long cur_flow = 0;

    do 
    {
        std::fill(used.begin(), used.end(), false);
        cur_flow = depthFirstSearch(graph, used, source, sink, INF);
        max_flow += cur_flow;
    }
    while (cur_flow != 0);

    return max_flow;
}

int main()
{
    size_t vertex_number = 0;
    size_t edge_number   = 0;
    std::cin >> vertex_number >> edge_number;

    Graph graph(vertex_number);

    for (size_t edge_id = 0; edge_id < edge_number; ++edge_id)
    {
        size_t start = 0, finish = 0;
        long long capacity = 0;

        std::cin >> start >> finish >> capacity;
        graph.addEdge(start-1, finish-1, capacity);
    }

    long long result = findMaximalFlow(graph, 0, vertex_number-1);
    std::cout << result << '\n';

    return 0;
}