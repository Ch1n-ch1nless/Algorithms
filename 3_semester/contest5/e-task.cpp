#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int start;
  int finish;
  int cost;
};

using Graph = std::vector<Edge>;

const int INF = __INT_MAX__;

class NegativeWeightCycleDetector {
 public:
  NegativeWeightCycleDetector(const Graph& graph, size_t vertex_number);
  ~NegativeWeightCycleDetector() = default;

  std::vector<int>& getVertexListInCycle();
  const std::vector<int>& getVertexListInCycle() const;

 private:
  void findNegativeWeightCycleByBellmanFord(const Graph& graph,
                                            size_t vertex_number);

  bool is_graph_contain_neg_cycle_;
  int  cycle_start_vertex_;
  std::vector<int> vertex_list_;
};

void NegativeWeightCycleDetector::findNegativeWeightCycleByBellmanFord(
    const Graph& graph, size_t vertex_number) {
  size_t try_number = 0;
  bool   is_updated = true;

  std::vector<int> distances(vertex_number, INF);
  std::vector<int> parent(vertex_number, -1);

  while (is_updated && (try_number <= vertex_number)) {
    is_updated = false;

    for (auto cur_edge : graph) {
      int start  = cur_edge.start;
      int finish = cur_edge.finish;
      int cost   = cur_edge.cost;

      if (distances[start] != INF && (distances[start] + cost < distances[finish]))
      {
          distances[finish] = distances[start] + cost;
          parent[finish] = start;
          is_updated = true;

          if (try_number == vertex_number) {
            cycle_start_vertex_ = start;
            break;
          }
      }
      try_number++;
    }   
  }

  if (is_updated) {
    
  }
}