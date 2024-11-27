#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Edge
{
    int to;
    int cost;
};

template <>
bool std::greater<Edge>::operator()(const Edge& lhs, const Edge& rhs) const
{
    return lhs.cost > rhs.cost;
}

using Graph = std::vector<std::vector<Edge>>;

const int INF = 2009000999;

std::vector<int> findShortestPathDijkstra(Graph& graph, int start_vertex)
{
    size_t vertex_number = graph.size();

    std::vector<int> distance(vertex_number, INF);

    distance[start_vertex] = 0;

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> queue = {};

    queue.push(Edge{start_vertex, 0});

    while (!queue.empty())
    {
        Edge cur_edge = queue.top();
        queue.pop();

        if (cur_edge.cost > distance[cur_edge.to]) continue;

        for (Edge out_edge : graph[cur_edge.to])
        {
            int to   = out_edge.to;
            int cost = out_edge.cost;

            if (distance[cur_edge.to] + cost < distance[to])
            {
                distance[to] = distance[cur_edge.to] + cost;
                queue.push(Edge{to, distance[to]});
            }
        }
    }

    return distance;
}

int main()
{
    size_t location_number = 0;
    std::cin >> location_number;

    for (size_t location_id = 0; location_id < location_number; ++location_id)
    {
        size_t vertex_number = 0;
        size_t edge_number   = 0;

        std::cin >> vertex_number >> edge_number;

        Graph graph(vertex_number);

        for (size_t edge_id = 0; edge_id < edge_number; ++edge_id)
        {
            int start_vertex  = 0;
            int finish_vertex = 0;
            int cost          = 0;

            std::cin >> start_vertex >> finish_vertex >> cost;

            graph[start_vertex].push_back(Edge{finish_vertex, cost});
            graph[finish_vertex].push_back(Edge{start_vertex, cost});
        }

        int start_vertex = 0;
        std::cin >> start_vertex;

        std::vector<int> distances = findShortestPathDijkstra(graph, start_vertex);

        for (auto dist_cost : distances)
        {
            std::cout << dist_cost << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}