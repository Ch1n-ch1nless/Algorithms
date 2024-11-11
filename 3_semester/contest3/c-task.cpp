#include <iostream>
#include <vector>

void FindComponent(std::vector<std::vector<int>>& graph, int vertex, std::vector<bool>& used, std::vector<int>& component)
{
    component.push_back(vertex);
    used[vertex] = true;

    for (int new_vertex : graph[vertex])
    {
        if (!used[new_vertex])
        {
            FindComponent(graph, new_vertex, used, component);
        }
    }
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
        graph[v2].push_back(v1);
    }

    std::vector<bool> used(vertex_number + 1, false);

    std::vector<std::vector<int>> components(vertex_number + 1);

    int component_number = 0;

    for (int v = 1; v <= vertex_number; ++v)
    {
        if (!used[v])
        {
            FindComponent(graph, v, used, components[component_number]);
            component_number++;
        }
    }

    std::cout << component_number << '\n';

    for (int i = 0; i < component_number; ++i)
    {
        std::cout << components[i].size() << '\n';

        for (int vertex : components[i])
        {
            std::cout << vertex << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}