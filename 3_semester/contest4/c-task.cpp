#include <algorithm>
#include <iostream>
#include <vector>

const int INF = __INT_MAX__;

int FindPrimMST(int vertex_number, const std::vector<std::vector<int>> &graph)
{
    std::vector<bool> used(vertex_number, false);
    std::vector<int>  min_edge(vertex_number, INF);
    std::vector<int>  sel_e(vertex_number, -1);

    for (int i = 0; i < vertex_number; i++)
    {
        int cur_vertex = -1;

        for (int j = 0; j < vertex_number; j++)
        {
            if (!used[j] && (cur_vertex == -1 || (min_edge[j] < min_edge[cur_vertex])))
            {
                cur_vertex = j;
            }
        }

        used[cur_vertex] = true;

        for (int next = 0; next < vertex_number; next++)
        {
            if (!used[next] && graph[cur_vertex][next] <  min_edge[next])
            {
                min_edge[next] = graph[cur_vertex][next];
                sel_e[next]    = cur_vertex;
            }
        }
    }

    int min_cost = 0;

    for (int i = 0; i < vertex_number; i++)
    {
        if (sel_e[i] != -1)
        {
            min_cost += graph[i][sel_e[i]];
        }
    }

    return min_cost;
}

int main()
{
    int vertex_number = 0;
    std::cin >> vertex_number;

    std::vector<std::vector<int>> graph(vertex_number + 1, std::vector<int>(vertex_number + 1, INF));

    for (int i = 0; i < vertex_number; i++)
    {
        for (int j = 0; j < vertex_number; j++)
        {
            std::cin >> graph[i][j];
        }
    }

    for (int i = 0; i < vertex_number; i++)
    {
        std::cin >> graph[i][vertex_number];
        graph[vertex_number][i] = graph[i][vertex_number];
    }

    for (int i = 0; i < vertex_number; i++)
    {
        graph[i][i] = INF;
    }

    int answer = FindPrimMST(vertex_number+1, graph);

    std::cout << answer << '\n';

    return 0;
}