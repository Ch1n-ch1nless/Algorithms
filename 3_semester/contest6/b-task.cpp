#include <algorithm>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<long long>>;

bool depthFirstSearch(const Graph             &graph,
                      std::vector<bool>       &used,
                      std::vector<long long>  &matching,
                      long long                begin    )
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

long long findGraphMatching(Graph& graph, long long m, long long n)
{
    std::vector<bool> used(m * n, false);
    std::vector<long long> matching(m * n, -1);

    for (long long i = 0; i < m; i++)
    {
        for (long long j = 0; j < n; j++)
        {
            if ((i + j) % 2 == 1) continue;
            if (depthFirstSearch(graph, used, matching, i * m + j) == true) 
            {
                std::fill(used.begin(), used.end(), false);
            }
        }
    }

    long long matchings_number = 0;
    for (long long to = 0; to < m * n; to++)
    {
        if (matching[to] != -1)
        {
            ++matchings_number;
        }
    }

    return matchings_number;
}

int main()
{
    long long m = 0;
    long long n = 0;
    long long a = 0;
    long long b = 0;

    std::cin >> m >> n >> a >> b;

    std::vector<std::vector<char>> field(m, std::vector<char>(n, '\0'));
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; j++)
        {
            std::cin >> field[i][j];
        }
    }

    Graph graph(n * m);
    long long empty_cell_number = 0;

    for (long long i = 0; i < m; ++i)
    {
        for (long long j = 0; j < n; j++)
        {
            long long cur_vertex = i * m + j;

            if (field[i][j] == '.') continue;

            empty_cell_number++;
            if ((i + j) % 2 == 1) continue;

            if ((i > 0)     && field[i-1][j] == '*')    graph[cur_vertex].push_back(cur_vertex - m);
            if ((j > 0)     && field[i][j-1] == '*')    graph[cur_vertex].push_back(cur_vertex - 1);
            if ((j < n - 1) && field[i][j+1] == '*')    graph[cur_vertex].push_back(cur_vertex + 1);
            if ((i < m - 1) && field[i+1][j] == '*')    graph[cur_vertex].push_back(cur_vertex + m);

        }
    }

    if (2 * b <= a)
    {
        std::cout << b * empty_cell_number << '\n';
        return 0;
    }

    long long matching_number = findGraphMatching(graph, m, n);

    long long answer = a * matching_number + 
                       b * (empty_cell_number - 2 * matching_number);

    std::cout << answer << '\n';

    return 0;
}