#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int start;
  int finish;
  int cost;
};

using Graph = std::vector<Edge>;

const int INF = 100000;

class NegativeWeightCycleDetector {
 public:
  NegativeWeightCycleDetector(const Graph& graph, size_t vertex_number);
  ~NegativeWeightCycleDetector() = default;

  bool isNegativeCycleDetected() const;

  std::vector<int>& getVertexListInCycle();
  const std::vector<int>& getVertexListInCycle() const;

 private:
  void findNegativeWeightCycleByBellmanFord(const Graph& graph,
                                            size_t vertex_number);

  void restoreVertexCycle(std::vector<int>& parent);

  bool is_graph_contains_neg_cycle_ = false;
  int cycle_start_vertex_ = -1;
  std::vector<int> vertex_cycle_;
};

void NegativeWeightCycleDetector::findNegativeWeightCycleByBellmanFord(
    const Graph& graph, size_t vertex_number) {
  size_t try_number = 0;
  bool is_updated = true;

  std::vector<int> distances(vertex_number, INF);
  std::vector<int> parent(vertex_number, -1);

  while (is_updated && (try_number <= vertex_number)) {
    is_updated = false;

    for (auto cur_edge : graph) {
      int start = cur_edge.start;
      int finish = cur_edge.finish;
      int cost = cur_edge.cost;

      if ((distances[start] + cost) < distances[finish]) {
        distances[finish] = distances[start] + cost;
        parent[finish] = start;
        is_updated = true;

        if (try_number == vertex_number) {
          cycle_start_vertex_ = start;
          break;
        }
      }
    }
    try_number++;
  }

  if (is_updated) {
    is_graph_contains_neg_cycle_ = true;
    restoreVertexCycle(parent);
    return;
  }

  is_graph_contains_neg_cycle_ = false;
  return;
}

void NegativeWeightCycleDetector::restoreVertexCycle(std::vector<int>& parent) {
  int v = cycle_start_vertex_;
  for (size_t i = 0; i < parent.size(); i++) {
    v = parent[v];
  }
  int u = v;
  v = parent[v];
  while (u != v) {
    vertex_cycle_.push_back(v);
    v = parent[v];
  }
  vertex_cycle_.push_back(v);
  vertex_cycle_.push_back(parent[v]);

  std::reverse(vertex_cycle_.begin(), vertex_cycle_.end());
}

NegativeWeightCycleDetector::NegativeWeightCycleDetector(const Graph& graph,
                                                         size_t vertex_number)
    : is_graph_contains_neg_cycle_(false),
      cycle_start_vertex_(-1),
      vertex_cycle_() {
  findNegativeWeightCycleByBellmanFord(graph, vertex_number);
}

bool NegativeWeightCycleDetector::isNegativeCycleDetected() const {
  return is_graph_contains_neg_cycle_;
}

std::vector<int>& NegativeWeightCycleDetector::getVertexListInCycle() {
  return vertex_cycle_;
}

const std::vector<int>& NegativeWeightCycleDetector::getVertexListInCycle()
    const {
  return vertex_cycle_;
}

int main() {
  int vertex_number = 0;
  std::cin >> vertex_number;

  Graph graph;

  for (int start_vertex = 0; start_vertex < vertex_number; start_vertex++) {
    for (int end_vertex = 0; end_vertex < vertex_number; end_vertex++) {
      int weight = 0;
      std::cin >> weight;
      if (weight != INF)
        graph.push_back(Edge{start_vertex, end_vertex, weight});
    }
  }

  NegativeWeightCycleDetector cycle_detector(graph, vertex_number);

  if (cycle_detector.isNegativeCycleDetected() == false) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";
    std::vector<int> vertex_negative_cycle =
        cycle_detector.getVertexListInCycle();
    std::cout << vertex_negative_cycle.size() << '\n';
    for (auto vertex : vertex_negative_cycle) {
      std::cout << vertex + 1 << ' ';
    }
    std::cout << '\n';
  }

  return 0;
}