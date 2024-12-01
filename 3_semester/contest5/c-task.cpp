#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

const int INF = __INT_MAX__;

struct Edge
{
    int to;
    int cost;
    int time;
};

template<>
bool std::greater<Edge>::operator()(const Edge& lhs, const Edge& rhs) const
{
    if (lhs.cost == rhs.cost)
    {
        if (lhs.time == rhs.time)
        {
            return lhs.to > rhs.to;
        }
        return lhs.time > rhs.time;
    }
    return lhs.cost > rhs.cost;
}

using Graph = std::vector<std::vector<Edge>>;

class TraitorDetector
{
public:
     TraitorDetector(Graph& src_graph, int max_time);
    ~TraitorDetector() = default;

    bool             isTraitorDiscovered();
    int              getMinCost();
    std::vector<int> getPathToTraitor();

private:
    std::vector<std::vector<int>>   min_path_cost_dp_;
    std::vector<std::vector<int>>   parent_;
    std::vector<int>                traitor_path_;
    int                             min_cost_;
    bool                            is_traitor_detected_;

    void findShortestPathDijkstra(Graph& src_graph, int max_time, int start_vertex, int finish_vertex);
    void restorePath(Graph& graph, int best_time, int start_vertex);
};

TraitorDetector::TraitorDetector(Graph& graph, int max_time)
:
    min_path_cost_dp_(graph.size(), std::vector<int>(max_time + 1, INF)),
    parent_(graph.size(), std::vector<int>(max_time + 1, -1)),
    traitor_path_(),
    min_cost_(INF),
    is_traitor_detected_(false)
{
    findShortestPathDijkstra(graph, max_time, 1, graph.size()-1);
}

void TraitorDetector::findShortestPathDijkstra(Graph& src_graph, int max_time, int start_vertex, int finish_vertex)
{
    min_path_cost_dp_[start_vertex][0] = 0;

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> queue;
    queue.push(Edge{start_vertex, 0, 0});

    while (!queue.empty())
    {
        Edge cur_edge = queue.top();
        queue.pop();

        if (cur_edge.cost > min_path_cost_dp_[cur_edge.to][cur_edge.time])
        {
            continue;
        }

        for (Edge& new_edge : src_graph[cur_edge.to])
        {
            int next_vertex = new_edge.to;
            int new_cost    = cur_edge.cost + new_edge.cost;
            int new_time    = cur_edge.time + new_edge.time;

            if (new_time > max_time) 
            {
                continue;
            }

            if (new_cost < min_path_cost_dp_[next_vertex][new_time])
            {
                min_path_cost_dp_[next_vertex][new_time] = new_cost;
                parent_[next_vertex][new_time] = cur_edge.to;
                queue.push(Edge{next_vertex, new_cost, new_time});
            }
        }
    }

    int best_time = -1;

    for (int time = 0; time <= max_time; ++time)
    {
        if (min_cost_ > min_path_cost_dp_[finish_vertex][time])
        {
            min_cost_ = min_path_cost_dp_[finish_vertex][time];
            best_time = time;
            is_traitor_detected_ = true;
        }
    }

    if (is_traitor_detected_)
    {
        restorePath(src_graph, best_time, finish_vertex);
    }
}

void TraitorDetector::restorePath(Graph& graph, int best_time, int start_vertex)
{
    int cur_vertex = start_vertex;
    int cur_time   = best_time;

    while (cur_vertex != -1)
    {
        traitor_path_.push_back(cur_vertex);
        int prev_vertex = parent_[cur_vertex][cur_time];
        if (prev_vertex != -1) {
            for (Edge& edge : graph[prev_vertex]) {
                if (edge.to == cur_vertex && cur_time >= edge.time) {
                    cur_time -= edge.time;
                    break;
                }
            }
        }
        cur_vertex = prev_vertex;
    }

    std::reverse(traitor_path_.begin(), traitor_path_.end());
}

bool TraitorDetector::isTraitorDiscovered()
{
    return is_traitor_detected_;
}

int TraitorDetector::getMinCost()
{
    return min_cost_;
}

std::vector<int> TraitorDetector::getPathToTraitor()
{
    return traitor_path_;
}

int main()
{
    int vertex_number = 0;
    int edge_number   = 0;
    int max_time      = 0;

    std::cin >> vertex_number >> edge_number >> max_time;

    Graph src_graph(vertex_number + 1);

    for (int edge_id = 0; edge_id < edge_number; ++edge_id)
    {
        int from = 0;
        int to   = 0;
        int cost = 0;
        int time = 0;

        std::cin >> from >> to >> cost >> time;

        src_graph[from].push_back(Edge{to, cost, time});
        src_graph[to].push_back(Edge{from, cost, time});
    }

    TraitorDetector solution(src_graph, max_time);

    if (solution.isTraitorDiscovered() == true)
    {
        std::cout << solution.getMinCost() << '\n';

        std::vector<int> path = solution.getPathToTraitor();
        
        std::cout << path.size() << '\n';
        for (int& vertex : path)
        {
            std::cout << vertex << ' ';
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << -1 << '\n';
    }

    return 0;
}