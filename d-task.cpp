#include <algorithm>
#include <iostream>
#include <vector>

class DisjointSetUnion {
 public:
  DisjointSetUnion(int set_number);
  ~DisjointSetUnion() = default;

  int getParent(int set_index);
  void uniteSets(int set1, int set2);

 private:
  std::vector<int> parent_;
  std::vector<int> size_;
};

DisjointSetUnion::DisjointSetUnion(int set_number)
    : parent_(set_number), size_(set_number, 1) {
  for (int i = 0; i < set_number; i++) {
    parent_[i] = i;
  }
}

int DisjointSetUnion::getParent(int set_index) {
  return (set_index == parent_[set_index])
             ? set_index
             : parent_[set_index] = getParent(parent_[set_index]);
}

void DisjointSetUnion::uniteSets(int set1, int set2) {
  set1 = getParent(set1);
  set2 = getParent(set2);

  if (set1 == set2) return;

  if (size_[set1] < size_[set2]) {
    std::swap(set1, set2);
  }

  size_[set1] += size_[set2];
  parent_[set2] = set1;
}

struct Edge {
  int begin;
  int end;
  int cost;
};

std::istream& operator>>(std::istream& in, Edge& edge)
{
  in >> edge.begin >> edge.end >> edge.cost;
  return in;
}

class Graph {
 public:
  Graph();
  Graph(int vertex_number);
  ~Graph() = default;

  void addEdge(const Edge& edge);

  std::vector<Edge>& operator[](int vertex);
  const std::vector<Edge>& operator[](int vertex) const;

  void setVertexNumber(int new_vertex_number);
  int  getVertexNumber() const;

 private:
  std::vector<std::vector<Edge>> adjacency_list_;
};

Graph::Graph() : adjacency_list_() {}

Graph::Graph(int vertex_number) : adjacency_list_(vertex_number) {}

void Graph::addEdge(const Edge& edge) {
  adjacency_list_[edge.begin].push_back(edge);
}

std::vector<Edge>& Graph::operator[](int vertex) {
  return adjacency_list_[vertex];
}

const std::vector<Edge>& Graph::operator[](int vertex) const {
  return adjacency_list_[vertex];
}

int Graph::getVertexNumber() const {
  return static_cast<int>(adjacency_list_.size());
}

void Graph::setVertexNumber(int new_vertex_number) {
  adjacency_list_.resize(new_vertex_number);
}

class MinimalSpanningTree {
 public:
  MinimalSpanningTree();

  ~MinimalSpanningTree() = default;

  Graph& getGraph();
  const Graph& getGraph() const;

  void build(const Graph& src);

 private:
  Graph tree_;
};

MinimalSpanningTree::MinimalSpanningTree() : tree_() {}

void MinimalSpanningTree::build(const Graph& src) {
  std::vector<Edge> edge_list = {};

  for (int i = 0; i < src.getVertexNumber(); ++i) {
    for (const Edge& new_edge : src[i]) {
      edge_list.push_back(new_edge);
    }
  }

  tree_.setVertexNumber(src.getVertexNumber());

  std::sort(edge_list.begin(), edge_list.end(), [](Edge& edge1, Edge& edge2){return edge1.cost > edge2.cost;});

  DisjointSetUnion dsu(src.getVertexNumber());

  for (std::size_t i = 0; i < edge_list.size(); ++i) {
    int begin = edge_list[i].begin;
    int end = edge_list[i].end;

    if (dsu.getParent(begin) != dsu.getParent(end)) {
      tree_[begin].push_back(edge_list[i]);
      tree_[end].push_back(
          Edge{edge_list[i].end, edge_list[i].begin, edge_list[i].cost});
      dsu.uniteSets(begin, end);
    }
  }
}

Graph& MinimalSpanningTree::getGraph() { return tree_; }

const Graph& MinimalSpanningTree::getGraph() const { return tree_; }

void DepthFirstSearch(const Graph& graph, std::vector<bool>& used,
                      std::vector<int>& dist, int cur_vertex) {
  used[cur_vertex] = true;

  for (auto& next : graph[cur_vertex]) {
    if (!used[next.end]) {
      dist[next.end] = std::min(dist[cur_vertex], next.cost);
      DepthFirstSearch(graph, used, dist, next.end);
    }
  }
}

int FindMaxCost(const Graph& graph, int begin, int end) {
  std::vector<bool> used(graph.getVertexNumber(), false);
  std::vector<int> dist(graph.getVertexNumber(), __INT_MAX__);

  DepthFirstSearch(graph, used, dist, begin);

  return dist[end];
}

void ReadGraph(Graph& graph, int edge_number) {
  for (int i = 0; i < edge_number; i++)
  {
    Edge new_edge = {};
    std::cin >> new_edge;

    graph.addEdge(new_edge);
    graph.addEdge(Edge{new_edge.end, new_edge.begin, new_edge.cost});
  }
}

int main() {
  std::cin.tie(NULL);
  std::ios_base::sync_with_stdio(false);

  int vertex_number = 0;
  int edge_number = 0;
  int request_number = 0;
  std::cin >> vertex_number >> edge_number >> request_number;

  Graph src_graph(vertex_number + 1);

  ReadGraph(src_graph, edge_number);

  MinimalSpanningTree mst;

  mst.build(src_graph);

  for (int i = 0; i < request_number; i++) {
    int vertex1 = 0;
    int vertex2 = 0;

    std::cin >> vertex1 >> vertex2;

    std::cout << FindMaxCost(mst.getGraph(), vertex1, vertex2) << '\n';
  }

  return 0;
}