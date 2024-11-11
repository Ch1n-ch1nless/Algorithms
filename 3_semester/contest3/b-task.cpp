#include <iostream>
#include <vector>

const char WHITE = 0;
const char GREY  = 1;
const char BLACK = 2;

const int NO_CYCLE      = -1;
const int CYCLE_FOUND   = 0;

int DepthFirstSearch(std::vector<std::vector<int>>& graph, int cur_vertex, std::vector<char>& used, std::vector<int>& path) 
{
    if (used[cur_vertex] != WHITE) {
        return CYCLE_FOUND;
    }

    used[cur_vertex] = GREY;
    path.push_back(cur_vertex);

    for (int new_vertex : graph[cur_vertex]) 
    {
        if (used[new_vertex] == WHITE) 
        {
            int res = DepthFirstSearch(graph, new_vertex, used, path);
            if (res != NO_CYCLE) return res;
        }
        else if (used[new_vertex] == GREY)
        {
            path.push_back(new_vertex);
            return CYCLE_FOUND;
        }
    }

    used[cur_vertex] = BLACK;
    path.pop_back();

    return NO_CYCLE;
}

int main()
{
    int vertex_number   = 0;
    int edge_number     = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<std::vector<int>> graph(vertex_number + 1);

    for (int i = 0; i < edge_number; ++i)
    {
        int v1 = 0;
        int v2 = 0;

        std::cin >> v1 >> v2;

        graph[v1].push_back(v2);
    }
    
    std::vector<char> used(vertex_number + 1, WHITE);
    std::vector<int> path = {};

    int result = 0;

    for (int vertex = 1; vertex < vertex_number; ++vertex)
    {
        if (used[vertex] == WHITE)
        {
            result = DepthFirstSearch(graph, vertex, used, path);
        }

        if (result != NO_CYCLE) break;
    }

    if (result == NO_CYCLE)
    {
        std::cout << "NO\n";
        return 0;
    }

    std::cout << "YES\n";

    int index       = path.size() - 2;
    int cycle_end   = path.back();

    while(path[index] != cycle_end) index--;

    for (; index < path.size() - 1; index++)
    {
        std::cout << path[index] << ' ';
    } 
    std::cout << '\n';

    return 0;
}