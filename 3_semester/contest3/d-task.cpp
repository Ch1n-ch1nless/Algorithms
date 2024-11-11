#include <algorithm>
#include <iostream>
#include <vector>

const char WHITE    = 0;
const char GREY     = 1;
const char BLACK    = 2;

bool IsComponentCyclic(const std::vector<std::vector<int>> &graph, int cur_vertex, std::vector<char>& used)
{
    used[cur_vertex] = GREY;

    for (const int new_vertex : graph[cur_vertex])
    {
        if (used[new_vertex] == WHITE)
        {
            bool res = IsComponentCyclic(graph, new_vertex, used);
            if (res) return res;
        }
        else if (used[new_vertex] == GREY)
        {
            return true;
        }
    }

    used[cur_vertex] = BLACK;

    return false;
}

bool IsGraphCyclic(const std::vector<std::vector<int>> &graph)
{
    std::vector<char> used(graph.size(), WHITE);

    bool result = false;

    for (int vertex = 1; vertex < graph.size(); ++vertex)
    {
        if (used[vertex] == WHITE)
        {
            result |= IsComponentCyclic(graph, vertex, used);
        }

        if (result) break;
    }

    return result;
}

void SortGraphComponent(const std::vector<std::vector<int>> &graph, int start, std::vector<bool> &used, std::vector<int> &vertex_array)
{
    used[start] = true;

    for (const int new_vertex : graph[start])
    {
        if (!used[new_vertex])
        {
            SortGraphComponent(graph, new_vertex, used, vertex_array);
        }
    }

    vertex_array.push_back(start);
}

std::vector<int> TopSort(const std::vector<std::vector<int>> &graph)
{
    std::vector<bool> used(graph.size(), false);

    std::vector<int> sorted_vertex_array = {};

    for (int v = 1; v < graph.size(); ++v)
    {
        if (!used[v])
        {
            SortGraphComponent(graph, v, used, sorted_vertex_array);
        }
    }

    std::reverse(sorted_vertex_array.begin(), sorted_vertex_array.end());

    return sorted_vertex_array;
}

int main()
{
    int vertex_number = 0;
    int edge_number   = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<std::vector<int>> graph(vertex_number + 1);

    for (int i = 0; i < edge_number; ++i)
    {
        int v1 = 0;
        int v2 = 0;

        std::cin >> v1 >> v2;

        graph[v1].push_back(v2);
    }

    bool is_cyclic_graph = IsGraphCyclic(graph);

    if (is_cyclic_graph)
    {
        std::cout << -1 << '\n';
        return 0;
    }

    std::vector<int> sorted_vertex_array = TopSort(graph);

    for (int vertex : sorted_vertex_array)
    {
        std::cout << vertex << ' ';
    }
    std::cout << '\n';

    return 0;
}