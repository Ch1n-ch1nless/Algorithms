#include <algorithm>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<int>>;

class SparseTable
{
public:
     SparseTable() = default;
    ~SparseTable() = default;

    void init(std::vector<int> &src_data);

    int  getMinIndex(int index1, int index2);

private:
    std::vector<std::vector<int>>   data_;
    std::vector<int>                log2_;

    size_t calcLogSize(size_t size);
};

size_t SparseTable::calcLogSize(size_t size)
{
    log2_.assign(size+1, 0);

    for (size_t i = 1; i <= size; i++)
    {
        log2_[i] = log2_[i/2] + 1;
    }

    return log2_[size];
}

void SparseTable::init(std::vector<int> &src_data)
{
    size_t max_height = calcLogSize(src_data.size());

    data_.assign(max_height, std::vector<int>(src_data.size(), 0));

    for (size_t i = 0; i < src_data.size(); ++i)
    {
        data_[0][i] = src_data[i];
    }

    for (size_t i = 1; i < max_height; i++)
    {
        for (size_t j = 0; j + (1 << i) - 1 < src_data.size(); j++)
        {
            data_[i][j] = std::min(data_[i-1][j], data_[i-1][j + (1 << (i - 1))]);
        }
    }
}

int SparseTable::getMinIndex(int left, int right)
{
    size_t height = log2_[right - left + 1] - 1;

    return std::min(data_[height][left], data_[height][right - (1 << height) + 1]);
}

class Solution
{
public:
     Solution(const Graph& graph);
    ~Solution() = default;

    int getDistBetweenSpecialists(int index1, int index2);

private:
    const Graph &tree;
    
    std::vector<int> dfs_list_;
    std::vector<int> depth_;
    std::vector<int> first_dfs_index_;

    class SparseTable rmq_;

    void fillDFSList();

    void dfs(std::vector<bool> &used, int cur_vertex, int depth);
};

Solution::Solution(const Graph& graph)
:
    tree(graph),
    first_dfs_index_(graph.size())
{
    fillDFSList();

    rmq_.init(depth_);
}

int Solution::getDistBetweenSpecialists(int index1, int index2)
{
    int i1 = first_dfs_index_[index1];
    int i2 = first_dfs_index_[index2];

    if (i1 > i2) std::swap(i1, i2);

    int anc_height = rmq_.getMinIndex(i1, i2);

    return abs(depth_[i1] - anc_height) + abs(depth_[i2] - anc_height);
}

void Solution::fillDFSList()
{
    std::vector<bool> used(tree.size(), false);

    for (int cur_vertex = 0; cur_vertex < tree.size(); cur_vertex++)
    {
        if (!used[cur_vertex])
        {
            dfs(used, cur_vertex, 0);
        }
    }
    /*
    for (auto v : dfs_list_)
    {
        std::cout << v << ' ';
    }
    std::cout << '\n';

    for (auto h : depth_)
    {
        std::cout << h << ' ';
    }
    std::cout << '\n';
    std::cout << '\n';*/
}

void Solution::dfs(std::vector<bool> &used, int cur_vertex, int depth)
{
    used[cur_vertex] = true;
    
    dfs_list_.push_back(cur_vertex);
    depth_.push_back(depth);

    first_dfs_index_[cur_vertex] = dfs_list_.size() - 1;

    for (int next_vertex : tree[cur_vertex])
    {
        if (!used[next_vertex])
        {
            dfs(used, next_vertex, depth+1);
            dfs_list_.push_back(cur_vertex);
            depth_.push_back(depth);
        }
    }
}

int main()
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int vertex_number = 0;
    int edge_number = 0;

    std::cin >> vertex_number;
    edge_number = vertex_number-1;

    Graph graph(vertex_number);

    for (int i = 0; i < edge_number; i++)
    {
        int v1 = 0;
        int v2 = 0;

        std::cin >> v1 >> v2;

        v1--;
        v2--;

        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
    }

    Solution solve(graph);

    int request_number = 0;

    std::cin >> request_number;

    for (int i = 0; i < request_number; i++)
    {
        int v1 = 0;
        int v2 = 0;

        std::cin >> v1 >> v2;

        v1--; v2--;

        std::cout << solve.getDistBetweenSpecialists(v1, v2) << '\n';
    }

    return 0;
}
