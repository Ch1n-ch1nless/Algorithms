#include <algorithm>
#include <iostream>
#include <vector>

struct Edge
{
    long long   to;
    long long   index;
    Edge*       back;
    bool        is_bridge;

    Edge() {}
    Edge(long long vertex, long long the_index) : 
        to      (vertex),
        index   (the_index),
        back    (nullptr)
    {}
};

void DepthFirstSearch(const std::vector<std::vector<Edge*>> &graph,
                      std::vector<bool>                     &used,
                      std::vector<long long>                &height,
                      std::vector<long long>                &up_height,
                      std::vector<long long>                &bridges,
                      long long                             vertex,
                      long long                             depth     = 0,
                      Edge*                                 prev_edge = nullptr)
{
    used[vertex] = true;

    height[vertex] = up_height[vertex] = depth;

    for (Edge* edge : graph[vertex])
    {
        if (!used[edge->to])
        {
            DepthFirstSearch(graph, used, height, up_height, bridges, edge->to, depth+1, edge);
            up_height[vertex] = std::min(up_height[vertex], up_height[edge->to]);
        }
        else if (prev_edge != nullptr && prev_edge->index != edge->index)
        {
            up_height[vertex] = std::min(up_height[vertex], height[edge->to]);
        }
    }

    if (prev_edge != nullptr && up_height[vertex] == height[vertex])
    {
        prev_edge->is_bridge        = true;
        prev_edge->back->is_bridge  = true;
        bridges.push_back(prev_edge->index);
    }

}

std::vector<long long> FindGraphBridges(const std::vector<std::vector<Edge*>> &graph)
{
    std::size_t vertex_number = graph.size();

    std::vector<bool>       used        (vertex_number, false);
    std::vector<long long>  depth       (vertex_number, 0ll);
    std::vector<long long>  up_height   (vertex_number, 0ll);

    std::vector<long long> bridges = {};

    for (long long vertex = 1; vertex < vertex_number; ++vertex)
    {
        if (!used[vertex])
        {
            DepthFirstSearch(graph, used, depth, up_height, bridges, vertex);
        }
    }

    std::sort(bridges.begin(), bridges.end());

    return bridges;
}

int main()
{
    long long vertex_number = 0;
    long long edge_number   = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<std::vector<Edge*>> graph(vertex_number + 1);

    for (long long i = 0; i < edge_number; ++i)
    {
        long long v1 = 0;
        long long v2 = 0;

        std::cin >> v1 >> v2;

        Edge* e12 = new Edge(v2, i+1);
        Edge* e21 = new Edge(v1, i+1);

        e12->back = e21;
        e21->back = e12;

        graph[v1].push_back(e12);
        graph[v2].push_back(e21);
    }

    std::vector<long long> bridges = FindGraphBridges(graph);

    std::cout << bridges.size() << '\n';
    for (long long edge_id : bridges)
    {
        std::cout << edge_id << ' ';
    }
    std::cout << '\n';

    //Clear memory
    for (auto edge_array : graph)
    {
        for (Edge* edge : edge_array)
        {
            if (edge != nullptr)
            {
                delete edge;
            }
            edge = nullptr;
        }
    }

    return 0;
}