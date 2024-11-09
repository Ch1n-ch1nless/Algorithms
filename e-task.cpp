#include <algorithm>
#include <iostream>
#include <vector>

void TopSortComponent(const std::vector<std::vector<int>> &graph,
                      std::vector<int> &order, std::vector<bool> &used,
                      int vertex) {
  used[vertex] = true;

  for (const int &next : graph[vertex]) {
    if (!used[next]) {
      TopSortComponent(graph, order, used, next);
    }
  }

  order.push_back(vertex);
}

void TopSortGraph(const std::vector<std::vector<int>> &graph,
                  std::vector<int> &order) {
  std::vector<bool> used(graph.size(), false);

  for (int vertex = 1; vertex < graph.size(); ++vertex) {
    if (!used[vertex]) {
      TopSortComponent(graph, order, used, vertex);
    }
  }
}

void FindComponent(const std::vector<std::vector<int>> &graph,
                   std::vector<int> &components, int comp_count, int vertex) {
  components[vertex] = comp_count;

  for (const int &next : graph[vertex]) {
    if (components[next] == 0) {
      FindComponent(graph, components, comp_count, next);
    }
  }
}

int FindStronglyConnectedComponents(
    const std::vector<std::vector<int>> &graph,
    const std::vector<std::vector<int>> &transposed_graph,
    std::vector<int> &strongly_connected_components) {
  std::vector<int> vertex_order = {};

  TopSortGraph(graph, vertex_order);

  std::reverse(vertex_order.begin(), vertex_order.end());

  strongly_connected_components.assign(graph.size(), 0);

  int components_count = 0;

  for (int &vertex : vertex_order) {
    if (strongly_connected_components[vertex] == 0) {
      components_count++;
      FindComponent(transposed_graph, strongly_connected_components,
                    components_count, vertex);
    }
  }

  return components_count;
}

int main() {
  int vertex_number = 0;
  int edge_number = 0;

  std::cin >> vertex_number >> edge_number;

  std::vector<std::vector<int>> graph(vertex_number + 1);
  std::vector<std::vector<int>> transposed_graph(vertex_number + 1);

  for (int i = 0; i < edge_number; i++) {
    int v1 = 0;
    int v2 = 0;

    std::cin >> v1 >> v2;

    graph[v1].push_back(v2);
    transposed_graph[v2].push_back(v1);
  }

  std::vector<int> strongly_connected_components = {};

  int components_number = FindStronglyConnectedComponents(
      graph, transposed_graph, strongly_connected_components);

  std::cout << components_number << '\n';

  for (int vertex_id = 1; vertex_id <= vertex_number; vertex_id++) {
    std::cout << strongly_connected_components[vertex_id] << ' ';
  }
  std::cout << '\n';

  return 0;
}