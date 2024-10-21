#include <iostream>
#include <set>
#include <vector>

void DepthFirstSearch(const std::vector<std::vector<long long>> &graph,
                      std::vector<bool>                         &used,
                      std::vector<long long>                    &depth,
                      std::vector<long long>                    &height,
                      std::set<long long>                       &articulation_points,
                      long long                                 cur_vertex,
                      long long                                 previous = -1ll)
{
    used[cur_vertex] = true;

    depth[cur_vertex] = height[cur_vertex] = (previous == -1) ? 0 : height[previous] + 1;

    long long children = 0;

    for (long long new_vertex : graph[cur_vertex])
    {
        if (new_vertex != previous)
        {
            if (!used[new_vertex])
            {
                DepthFirstSearch(graph, used, depth, height, articulation_points, new_vertex, cur_vertex);
                depth[cur_vertex] = std::min(depth[cur_vertex], depth[new_vertex]);
                
                if (height[cur_vertex] <= depth[new_vertex] && previous != -1)
                {
                    articulation_points.insert(cur_vertex);
                }

                children++;
            }
            else
            {
                depth[cur_vertex] = std::min(depth[cur_vertex], height[new_vertex]);
            }
        }
    }

    if (previous == -1 && children > 1)
    {
        articulation_points.insert(cur_vertex);
    }
}

std::set<long long> FindArticulationPoints(const std::vector<std::vector<long long>> &graph)
{
    std::vector<bool>       used   (graph.size(), false);
    std::vector<long long>  depth  (graph.size(), 0ll);
    std::vector<long long>  height (graph.size(), 0ll);

    std::set<long long> articulation_points = {};

    for (long long vertex = 1; vertex < graph.size(); ++vertex)
    {
        if (!used[vertex])
        {
            DepthFirstSearch(graph, used, depth, height, articulation_points, vertex);
        }
    }

    return articulation_points;
}

int main()
{
    long long vertex_number = 0;
    long long edge_number   = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<std::vector<long long>> graph(vertex_number + 1);

    for (long long i = 0; i < edge_number; ++i)
    {
        long long v1 = 0;
        long long v2 = 0;

        std::cin >> v1 >> v2;

        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
    }

    std::set<long long> articulation_points = FindArticulationPoints(graph);

    std::cout << articulation_points.size() << '\n';

    for (auto vertex : articulation_points)
    {
        std::cout << vertex << '\n';
    }

    return 0;
}