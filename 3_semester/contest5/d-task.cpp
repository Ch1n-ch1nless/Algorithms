#include <algorithm>
#include <iostream>
#include <vector>

struct Edge
{
    int start;
    int finish;
    int cost;
};

using Graph = std::vector<Edge>;

const int INF = 30000;

std::vector<int> findShortestPathBellmanFord(const Graph& graph, size_t vertex_number)
{
    std::vector<int> distances(vertex_number, INF);
    distances[0] = 0;

    for (size_t i = 0; i < (vertex_number - 1); ++i)
    {
        for (auto cur_edge : graph)
        {
            int start  = cur_edge.start;
            int finish = cur_edge.finish;
            int cost   = cur_edge.cost;

            if (distances[start] != INF && (distances[start] + cost < distances[finish]))
            {
                distances[finish] = distances[start] + cost;
            }
        }
    }

    return distances;
}

int main()
{
    size_t vertex_number = 0;
    size_t edge_number   = 0;

    std::cin >> vertex_number >> edge_number;

    Graph graph = {};

    for (size_t edge_id = 0; edge_id < edge_number; ++edge_id)
    {
        int start  = 0;
        int finish = 0;
        int cost   = 0;

        std::cin >> start >> finish >> cost;

       --start; --finish;

        graph.push_back(Edge{start, finish, cost});
    }

    std::vector<int> distances = findShortestPathBellmanFord(graph, vertex_number);

    for (auto dist : distances)
    {
        std::cout << dist << ' ';
    }
    std::cout << '\n';

    return 0;
}