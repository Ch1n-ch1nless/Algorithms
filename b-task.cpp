#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using Vertex = int;

struct Edge {
  int to;
  long long cost;
};

template <>
bool std::greater<Edge>::operator()(const Edge& lhs, const Edge& rhs) const {
  return lhs.cost > rhs.cost;
}

class Graph {
 public:
  Graph() = default;
  Graph(size_t vertex_number);
  Graph(const Graph& other);

  Graph& operator=(const Graph& other);

  std::vector<Edge>& operator[](const Vertex& vertex);
  const std::vector<Edge>& operator[](const Vertex& vertex) const;

  size_t getVertexNum() const;
  void setVertexNum(size_t new_vertex_num);

  void addEdge(const Vertex& start, const Vertex& finish, long long cost);

 private:
  std::vector<std::vector<Edge>> adjacency_list_;
};

Graph::Graph(size_t vertex_number) : adjacency_list_(vertex_number) {}

Graph::Graph(const Graph& other) : adjacency_list_(other.adjacency_list_) {}

Graph& Graph::operator=(const Graph& other) {
  if (this != &other) {
    adjacency_list_ = other.adjacency_list_;
  }
  return *this;
}

std::vector<Edge>& Graph::operator[](const Vertex& vertex) {
  return adjacency_list_[vertex];
}

const std::vector<Edge>& Graph::operator[](const Vertex& vertex) const {
  return adjacency_list_[vertex];
}

size_t Graph::getVertexNum() const { return adjacency_list_.size(); }

void Graph::setVertexNum(size_t new_vertex_num) {
  adjacency_list_.resize(new_vertex_num);
}

void Graph::addEdge(const Vertex& start, const Vertex& finish, long long cost) {
  adjacency_list_[start].push_back(Edge{finish, cost});
  adjacency_list_[finish].push_back(Edge{start, cost});
}

const long long INF = __LONG_LONG_MAX__;

std::vector<long long> findShortestPathDijkstra(const Graph& graph,
                                                int start_vertex) {
  size_t vertex_number = graph.getVertexNum();

  std::vector<long long> distance(vertex_number, INF);

  distance[start_vertex] = 0;

  std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> queue = {};

  queue.push(Edge{start_vertex, 0});

  while (!queue.empty()) {
    Edge cur_edge = queue.top();
    queue.pop();

    if (cur_edge.cost > distance[cur_edge.to]) continue;

    for (Edge out_edge : graph[cur_edge.to]) {
      int to = out_edge.to;
      int cost = out_edge.cost;

      if (distance[cur_edge.to] + cost < distance[to]) {
        distance[to] = distance[cur_edge.to] + cost;
        queue.push(Edge{to, distance[to]});
      }
    }
  }

  return distance;
}

long long findMinTimeToMedicalRoom(const Graph& graph, int start_vertex, int finish_vertex,
                     const std::vector<int>& virus) {
  std::vector<long long> distance =
      findShortestPathDijkstra(graph, finish_vertex);
  long long virus_min_path = INF;

  for (auto virus_vertex : virus) {
    virus_min_path = std::min(virus_min_path, distance[virus_vertex]);
  }

  long long ans = distance[start_vertex];

  return (ans >= virus_min_path) ? -1 : ans;
}

int main() {
  size_t vertex_number = 0;
  size_t edge_number = 0;
  size_t virus_number = 0;

  std::cin >> vertex_number >> edge_number >> virus_number;

  Graph graph(vertex_number + 1);

  std::vector<int> viruses(virus_number);

  for (size_t i = 0; i < virus_number; ++i) {
    std::cin >> viruses[i];
  }

  for (size_t edge_id = 0; edge_id < edge_number; ++edge_id) {
    int start_vertex = 0;
    int finish_vertex = 0;
    long long cost = 0;

    std::cin >> start_vertex >> finish_vertex >> cost;

    graph.addEdge(start_vertex, finish_vertex, cost);
  }

  int start_vertex = 0;
  int finish_vertex = 0;

  std::cin >> start_vertex >> finish_vertex;

  std::cout << findMinTimeToMedicalRoom(graph, start_vertex, finish_vertex, viruses) << '\n';

  return 0;
}