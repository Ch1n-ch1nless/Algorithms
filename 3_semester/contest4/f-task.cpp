#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int begin;
  int end;
};

class Graph {
 public:
  Graph();
  Graph(int vertex_number);
  ~Graph() = default;

  void addEdge(const Edge& edge);

  std::vector<int>& operator[](int vertex);
  const std::vector<int>& operator[](int vertex) const;

  int getVertexNumber() const;
  void setVertexNumber(int vertex_number);

 private:
  std::vector<std::vector<int>> adjacency_list_;
};

Graph::Graph() : adjacency_list_() {}

Graph::Graph(int vertex_number) : adjacency_list_(vertex_number) {}

void Graph::addEdge(const Edge& edge) {
  adjacency_list_[edge.begin].push_back(edge.end);
  adjacency_list_[edge.end].push_back(edge.begin);
}

std::vector<int>& Graph::operator[](int vertex) {
  return adjacency_list_[vertex];
}

const std::vector<int>& Graph::operator[](int vertex) const {
  return adjacency_list_[vertex];
}

int Graph::getVertexNumber() const {
  return static_cast<int>(adjacency_list_.size());
}

void Graph::setVertexNumber(int vertex_number) {
  adjacency_list_.resize(vertex_number);
}

class CondensedGraph {
 public:
  CondensedGraph(const Graph& src_graph);
  ~CondensedGraph() = default;

  Graph& getTree();
  const Graph& getTree() const;

  int getComponentId(int src_vertex);

 private:
  Graph tree_;
  Graph temp_new_graph_;

  std::vector<int> components_;

  std::vector<int> tin;
  std::vector<int> ret;
  std::vector<bool> used;
  std::vector<Edge> bridges;
  int timer = 0;

  void findAllBridges(const Graph& src_graph);
  void findBridge(const Graph& src_graph, int vertex, int parent = -1);

  int splitGraphToComponents();
  void findAllVertexInComponent(int vertex, int component_id);

  void condensateGraph();
};

CondensedGraph::CondensedGraph(const Graph& src_graph) {
  temp_new_graph_.setVertexNumber(src_graph.getVertexNumber());
  findAllBridges(src_graph);
  condensateGraph();

  /*std::cout << "Vertex    := { ";
  for (int i = 0; i < components_.size(); i++)
  {
    std::cout << i << ", ";
  }
  std::cout << "}\n";

  std::cout << "Component := { ";
  for (int i = 0; i < components_.size(); i++)
  {
    std::cout << components_[i] << ", ";
  }
  std::cout << "}\n";*/
}

Graph& CondensedGraph::getTree() { return tree_; }

const Graph& CondensedGraph::getTree() const { return tree_; }

int CondensedGraph::getComponentId(int src_vertex) {
  return components_[src_vertex];
}

void CondensedGraph::findAllBridges(const Graph& src_graph) {
  used.resize(src_graph.getVertexNumber(), false);
  timer = 0;
  tin.resize(src_graph.getVertexNumber());
  ret.resize(src_graph.getVertexNumber());
  bridges = {};

  for (int i = 0; i < src_graph.getVertexNumber(); ++i) {
    if (!used[i]) {
      findBridge(src_graph, i, -1);
    }
  }
}

void CondensedGraph::findBridge(const Graph& src_graph, int v, int p) {
  tin[v] = timer++;
  ret[v] = tin[v];
  used[v] = true;

  for (int to : src_graph[v]) {
    if (to == p) {
      continue;
    }

    if (used[to]) {
      ret[v] = std::min(ret[v], tin[to]);
    } else {
      findBridge(src_graph, to, v);
      ret[v] = std::min(ret[v], ret[to]);

      if (ret[to] > tin[v]) {
        bridges.push_back({v, to});
      } else {
        temp_new_graph_[v].push_back(to);
        temp_new_graph_[to].push_back(v);
      }
    }
  }
}

int CondensedGraph::splitGraphToComponents() {
  used.assign(temp_new_graph_.getVertexNumber(), false);
  components_.resize(temp_new_graph_.getVertexNumber());

  int components_num = 0;

  for (int i = 0; i < temp_new_graph_.getVertexNumber(); ++i) {
    if (!used[i]) {
      findAllVertexInComponent(i, components_num);
      ++components_num;
    }
  }

  return components_num;
}

void CondensedGraph::findAllVertexInComponent(int vertex, int component_id) {
  components_[vertex] = component_id;
  used[vertex] = true;

  for (int to : temp_new_graph_[vertex]) {
    if (!used[to]) {
      findAllVertexInComponent(to, component_id);
    }
  }
}

void CondensedGraph::condensateGraph() {
  int components_number = splitGraphToComponents();

  tree_.setVertexNumber(components_number);

  for (const auto& bridge : bridges) {
    tree_[components_[bridge.begin]].push_back(components_[bridge.end]);
    tree_[components_[bridge.end]].push_back(components_[bridge.begin]);
  }
}

class SparseTable {
 public:
  SparseTable() = default;
  ~SparseTable() = default;

  void init(std::vector<int>& src_data);

  int getMinIndex(int index1, int index2);

 private:
  std::vector<std::vector<int>> data_;
  std::vector<int> log2_;

  size_t calcLogSize(size_t size);
};

size_t SparseTable::calcLogSize(size_t size) {
  log2_.assign(size + 1, 0);

  for (size_t i = 1; i <= size; i++) {
    log2_[i] = log2_[i / 2] + 1;
  }

  return log2_[size];
}

void SparseTable::init(std::vector<int>& src_data) {
  size_t max_height = calcLogSize(src_data.size());

  data_.assign(max_height, std::vector<int>(src_data.size(), 0));

  for (size_t i = 0; i < src_data.size(); ++i) {
    data_[0][i] = src_data[i];
  }

  for (size_t i = 1; i < max_height; i++) {
    for (size_t j = 0; j + (1 << i) - 1 < src_data.size(); j++) {
      data_[i][j] = std::min(data_[i - 1][j], data_[i - 1][j + (1 << (i - 1))]);
    }
  }
}

int SparseTable::getMinIndex(int left, int right) {
  size_t height = log2_[right - left + 1] - 1;

  return std::min(data_[height][left],
                  data_[height][right - (1 << height) + 1]);
}

class Solution {
 public:
  Solution(const Graph& graph, int start_vertex);
  ~Solution() = default;

  int getBrownRoadsNumber(int vertex1, int vertex2);

 private:
  CondensedGraph condensed_graph_;
  SparseTable rmq_;

  std::vector<int> dfs_list_;
  std::vector<int> depth_;
  std::vector<int> first_dfs_index_;

  void fillDFSList(const Graph& tree, int start_vertex);

  void dfs(const Graph& tree, std::vector<bool>& used, int cur_vertex,
           int depth);
};

Solution::Solution(const Graph& graph, int start_vertex)
    : condensed_graph_(graph) {
  first_dfs_index_.resize(condensed_graph_.getTree().getVertexNumber());

  fillDFSList(condensed_graph_.getTree(),
              condensed_graph_.getComponentId(start_vertex));

  /*for (auto vertex : dfs_list_)
  {
    std::cout << vertex << ' ';
  }
  std::cout << '\n';

  for (auto h : depth_)
  {
    std::cout << h << ' ';
  }
  std::cout << '\n';*/

  rmq_.init(depth_);
}

int Solution::getBrownRoadsNumber(int vertex1, int vertex2) {
  int index1 = condensed_graph_.getComponentId(vertex1);
  int index2 = condensed_graph_.getComponentId(vertex2);

  int i1 = first_dfs_index_[index1];
  int i2 = first_dfs_index_[index2];

  if (i1 > i2) std::swap(i1, i2);

  return rmq_.getMinIndex(i1, i2);
  ;
}

void Solution::fillDFSList(const Graph& tree, int start_vertex) {
  std::vector<bool> used(tree.getVertexNumber(), false);

  dfs(tree, used, start_vertex, 0);
}

void Solution::dfs(const Graph& tree, std::vector<bool>& used, int cur_vertex,
                   int depth) {
  used[cur_vertex] = true;

  dfs_list_.push_back(cur_vertex);
  depth_.push_back(depth);

  first_dfs_index_[cur_vertex] = dfs_list_.size() - 1;

  for (int next_vertex : tree[cur_vertex]) {
    if (!used[next_vertex]) {
      dfs(tree, used, next_vertex, depth + 1);
      dfs_list_.push_back(cur_vertex);
      depth_.push_back(depth);
    }
  }
}

int main() {
  std::cin.tie(NULL);
  std::ios_base::sync_with_stdio(false);

  int vertex_number = 0;
  int edge_number = 0;
  int final_vertex = 0;

  std::cin >> vertex_number >> edge_number >> final_vertex;
  final_vertex--;

  Graph src_graph(vertex_number);

  for (int i = 0; i < edge_number; i++) {
    int vertex1 = 0;
    int vertex2 = 0;

    std::cin >> vertex1 >> vertex2;

    vertex1--;
    vertex2--;

    src_graph.addEdge({vertex1, vertex2});
  }

  int request_number = 0;
  std::cin >> request_number;

  Solution solution(src_graph, final_vertex);

  for (int i = 0; i < request_number; i++) {
    int vertex1 = 0;
    int vertex2 = 0;

    std::cin >> vertex1 >> vertex2;

    vertex1--;
    vertex2--;

    std::cout << solution.getBrownRoadsNumber(vertex1, vertex2) << '\n';
  }

  return 0;
}