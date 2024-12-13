#include <algorithm>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<int>>;

struct Edge
{
    int begin;
    int end;
};

bool depthFirstSearch(const Graph       &graph,
                      std::vector<bool> &used,
                      std::vector<int>  &matching,
                      int                begin    )
{
    if (used[begin]) return false;

    used[begin] = true;

    for (auto& next : graph[begin])
    {
        if (matching[next] == -1 || 
            depthFirstSearch(graph, used, matching, matching[next]))
        {
            matching[next] = begin;
            return true;
        }
    }
    return false;
}

std::vector<Edge> findGraphMatchings(const Graph& graph, size_t left_block_size, size_t right_block_size)
{
    std::vector<bool> used(left_block_size, true);
    std::vector<int> matching(right_block_size, -1);

    for (int begin = 0; begin < graph.size(); ++begin)
    {
        std::fill(used.begin(), used.end(), false);
        depthFirstSearch(graph, used, matching, begin);
    }

    std::vector<Edge> matchings = {};
    for (int to = 0; to < right_block_size; to++)
    {
        if (matching[to] != -1)
        {
            matchings.push_back(Edge{matching[to], to});
        }
    }

    return matchings;
}

int main()
{
    size_t n = 0;
    size_t m = 0;
    std::cin >> n >> m;

    Graph graph(n);

    for (size_t begin = 0; begin < n; ++begin)
    {
        size_t end = 0;
        do 
        {
            std::cin >> end;

            if (end != 0) 
            {
                graph[begin].push_back(end-1);
            }
        } 
        while (end != 0);
    }

    std::vector<Edge> answer = 
        findGraphMatchings(graph, n, m);

    std::cout << answer.size() << '\n';
    for (auto& edge : answer)
    {
        std::cout << edge.begin + 1 << ' ' << edge.end + 1 << '\n';
    }

    return 0;
}