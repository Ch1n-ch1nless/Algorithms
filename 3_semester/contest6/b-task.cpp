#include <algorithm>
#include <iostream>
#include <vector>

struct Cell
{
    size_t row_index;
    size_t col_index;
};

class CellGraph
{
public:
    CellGraph(size_t rows, size_t columns);

    void    addEdge(const Cell& start, const Cell& finish);
    size_t  getVertexId(const Cell &cell) const;
    Cell    getCell(size_t vertex_id) const;

    size_t  getRowsNumber()    const { return rows_; }
    size_t  getColumnsNumber() const { return cols_; }

    std::vector<size_t> getAdjacencyVertices(size_t vertex) const;

private:
    std::vector<std::vector<size_t>> adjacency_list_;

    size_t rows_;
    size_t cols_;
};

CellGraph::CellGraph(size_t rows, size_t columns)
:
    adjacency_list_ (rows * columns),
    rows_           (rows),
    cols_           (columns)
{
}

void CellGraph::addEdge(const Cell& start, const Cell& finish)
{
    size_t start_id  = getVertexId(start);
    size_t finish_id = getVertexId(finish);

    adjacency_list_[start_id].push_back(finish_id);
}

size_t CellGraph::getVertexId(const Cell &cell) const
{
    return cell.row_index * cols_ + cell.col_index; 
}

Cell CellGraph::getCell(size_t vertex_id) const
{
    Cell result = {};
    result.row_index = vertex_id / cols_;
    result.col_index = vertex_id % cols_;
    return result;
}

std::vector<size_t> CellGraph::getAdjacencyVertices(size_t vertex) const
{
    return adjacency_list_[vertex];
}

bool depthFirstSearch(const CellGraph       &graph,
                      std::vector<bool>     &used,
                      std::vector<size_t>   &matching,
                      size_t                begin     )
{
    if (used[begin]) return false;

    used[begin] = true;

    for (auto& next : graph.getAdjacencyVertices(begin))
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

long long findGraphMatching(const CellGraph& graph)
{
    size_t rows     = graph.getRowsNumber();
    size_t columns  = graph.getColumnsNumber();

    std::vector<bool> used(rows * columns, false);
    std::vector<size_t> matching(rows * columns, -1);

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            if ((i + j) % 2 == 1) continue;
            if (depthFirstSearch(graph, used, matching, graph.getVertexId(Cell{i, j})) == true) 
            {
                std::fill(used.begin(), used.end(), false);
            }
        }
    }

    long long matchings_number = 0;
    for (long long to = 0; to < rows * columns; to++)
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
    size_t m = 0;
    size_t n = 0;
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

    CellGraph graph(m, n);

    long long empty_cell_number = 0;

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; j++)
        {
            if (field[i][j] == '.') continue;

            empty_cell_number++;
            if ((i + j) % 2 == 1) continue;

            if ((i > 0)     && field[i-1][j] == '*')    graph.addEdge(Cell{i, j}, Cell{i-1, j});
            if ((j > 0)     && field[i][j-1] == '*')    graph.addEdge(Cell{i, j}, Cell{i, j-1});
            if ((j < n - 1) && field[i][j+1] == '*')    graph.addEdge(Cell{i, j}, Cell{i, j+1});
            if ((i < m - 1) && field[i+1][j] == '*')    graph.addEdge(Cell{i, j}, Cell{i+1, j});

        }
    }

    if (2 * b <= a)
    {
        std::cout << b * empty_cell_number << '\n';
        return 0;
    }

    long long matching_number = findGraphMatching(graph);

    long long answer = a * matching_number + 
                       b * (empty_cell_number - 2 * matching_number);

    std::cout << answer << '\n';

    return 0;
}