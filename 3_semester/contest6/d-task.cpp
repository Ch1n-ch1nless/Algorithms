#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

struct Edge
{
    int from;
    int to;
    long long capacity;
    long long flow;
};

class Graph
{
public:
    Graph(size_t vertex_number);

    void addEdge(int start, int finish, long long capacity);

    std::vector<std::vector<int>>   edge_id;
    std::vector<Edge>               edges;
    size_t                          vertex_number;
};

Graph::Graph(size_t the_vertex_number)
:
    edge_id         (the_vertex_number),
    edges           (),
    vertex_number   (the_vertex_number)
{
}

void Graph::addEdge(int start, int finish, long long capacity)
{
    edge_id[start].push_back(edges.size());
    edges.push_back(Edge{start, finish, capacity, 0ll});

    edge_id[finish].push_back(edges.size());
    edges.push_back(Edge{finish, start, 0ll, 0ll});
}

const long long INF = __INT_MAX__;

class DinicAlgorithm
{
public:
     DinicAlgorithm() = default;
    ~DinicAlgorithm() = default;

    long long findMaxFlow(Graph &graph, int source, int sink);

private:
    bool        buildLevels(Graph &graph, int source, int sink);
    long long   findBlockedFlow(Graph &graph, int source, int sink, long long base_flow);

    std::vector<long long> level;
    std::vector<long long> pointer;
};

bool DinicAlgorithm::buildLevels(Graph &graph, int source, int sink)
{
    std::fill(level.begin(), level.end(), -1);
    level[source] = 0;
    
    std::queue<int> queue = {};
    queue.push(source);

    while (!queue.empty())
    {
        int vertex = queue.front();
        queue.pop();

        for (int id : graph.edge_id[vertex])
        {
            Edge &edge = graph.edges[id];

            if (level[edge.to] == -1 && edge.flow < edge.capacity)
            {
                level[edge.to] = level[vertex] + 1;
                queue.push(edge.to);
            }
        }
    }

    return level[sink] != -1;
}

long long DinicAlgorithm::findBlockedFlow(  Graph &graph, 
                                            int source, int sink, 
                                            long long base_capacity)
{
    if (!base_capacity || (source == sink)) return base_capacity;

    for (; pointer[source] < graph.edge_id[source].size(); ++pointer[source]) 
    {
		int id = graph.edge_id[source][pointer[source]];
		int to = graph.edges[id].to;

		if (level[to] != level[source] + 1)  continue;

		int pushed = findBlockedFlow(graph, to, sink, 
                                        std::min(base_capacity, 
                                                    graph.edges[id].capacity - 
                                                        graph.edges[id].flow));
		
        if (pushed) 
        {
			graph.edges[id].flow += pushed;
			graph.edges[id^1].flow -= pushed;
			return pushed;
		}
	}

    return 0;
}

long long DinicAlgorithm::findMaxFlow(Graph &graph, int source, int sink)
{
    level.resize(graph.vertex_number);
    pointer.resize(graph.vertex_number);

    long long max_flow = 0;

    while (buildLevels(graph, source, sink) == true)
    {
        std::fill(pointer.begin(), pointer.end(), 0ll);
        while (long long new_flow = 
                findBlockedFlow(graph, source, sink, INF))
        {
            max_flow += new_flow;
        }
    }

    return max_flow;
}

int main()
{
    size_t vertex_number = 0;
    size_t edge_number   = 0;
    std::cin >> vertex_number >> edge_number;

    Graph graph(vertex_number);

    std::vector<Edge> edges(edge_number);

    for (size_t edge_id = 0; edge_id < edge_number; edge_id++)
    {
        std::cin >> edges[edge_id].from >> edges[edge_id].to >> edges[edge_id].capacity;
        edges[edge_id].from  -= 1;
        edges[edge_id].to -= 1;

        graph.addEdge(edges[edge_id].from, edges[edge_id].to, edges[edge_id].capacity);
    }

    DinicAlgorithm algorithm = {};

    long long max_flow = algorithm.findMaxFlow(graph, 0, vertex_number-1);
    std::cout << max_flow << '\n';

    for (size_t i = 0; i < graph.edges.size(); i += 2)
    {
        std::cout << graph.edges[i].flow << '\n';
    }

    return 0;
}