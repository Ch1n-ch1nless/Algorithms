#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

struct Edge
{
    int to;
    long long cost;
};

template <>
bool std::greater<Edge>::operator()(const Edge& lhs, const Edge& rhs) const
{
    return lhs.cost > rhs.cost;
}

using Graph = std::vector<std::vector<Edge>>;

const long long INF = __LONG_LONG_MAX__;

std::vector<long long> findShortestPathDijkstra(const Graph& graph, int start_vertex)
{
    size_t vertex_number = graph.size();

    std::vector<long long> distance(vertex_number, INF);

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

long long findAnswer(const Graph& graph, int start_vertex, int finish_vertex, const std::vector<int> &virus)
{
    std::vector<long long> distance = findShortestPathDijkstra(graph, finish_vertex);
    long long virus_min_path = INF;

    for (auto virus_vertex : virus)
    {
        virus_min_path = std::min(virus_min_path, distance[virus_vertex]);
    }

    long long ans = distance[start_vertex];

    return (ans >= virus_min_path) ? -1 : ans;
}

int main()
{
    size_t vertex_number = 0;
    size_t edge_number   = 0;
    size_t virus_number  = 0;

    std::cin >> vertex_number >> edge_number >> virus_number;

    Graph graph(vertex_number + 1);

    std::vector<int> viruses(virus_number);

    for (size_t i = 0; i < virus_number; ++i)
    {
        std::cin >> viruses[i];
    }

    for (size_t edge_id = 0; edge_id < edge_number; ++edge_id)
    {
        int       start_vertex  = 0;
        int       finish_vertex = 0;
        long long cost          = 0;

        std::cin >> start_vertex >> finish_vertex >> cost;

        graph[start_vertex].push_back(Edge{finish_vertex, cost});
        graph[finish_vertex].push_back(Edge{start_vertex, cost});
    }

    int start_vertex  = 0;
    int finish_vertex = 0;

    std::cin >> start_vertex >> finish_vertex;

    std::cout << findAnswer(graph, start_vertex, finish_vertex, viruses) << '\n';

    return 0;
}