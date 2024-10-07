#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

static unsigned int INF = (std::numeric_limits<unsigned int>::max() / 4);

struct HamiltonianPath {
  unsigned int              min_cost;
  std::vector<unsigned int> vertex_sequence;
};

bool IsVertexInMask(unsigned int bit_mask, unsigned int vertex) {
  unsigned int vertex_mask = 1u << vertex;
  return ((bit_mask & vertex_mask) == vertex_mask) ? true : false;
}

HamiltonianPath FindHamiltonianPathWithMinCost(std::vector<std::vector<unsigned int>>& graph) {
  size_t max_bit_mask = (1 << graph.size()) - 1;

  std::vector<std::vector<unsigned int>> path_cost(graph.size(), std::vector<unsigned int>(max_bit_mask + 1, INF));
  std::vector<std::vector<unsigned int>> prev     (graph.size(), std::vector<unsigned int>(max_bit_mask + 1, INF));

  for (std::size_t i = 0; i < graph.size(); ++i) {
    path_cost[i][1u << i] = 0; 
  }

  /*std::cout << "==============\n";
  for (unsigned int vertex = 0; vertex < graph.size(); ++vertex) {
    for (unsigned int bit_mask = 0; bit_mask <= max_bit_mask; ++bit_mask) {
        std::cout << "path_cost[" << vertex << "][" << bit_mask << "] := " << path_cost[vertex][bit_mask] << '\n';
    }
    std::cout << "-------------\n";
  }
  std::cout << "==============\n\n";*/

  for (unsigned int bit_mask = 1; bit_mask <= max_bit_mask; ++bit_mask) {
    for (std::size_t v = 0; v < graph.size(); ++v) {
      if (IsVertexInMask(bit_mask, v)) {
        //std::cout << "bitmask := " << bit_mask << " | v := " << v << '\n';
        for (std::size_t u = 0; u < graph.size(); ++u) {
          if (!IsVertexInMask(bit_mask, u)) {
            //std::cout << "u := " << u << '\n';
            unsigned int& target = path_cost[u][bit_mask | (1u << u)];
            //std::cout << path_cost[u][bit_mask | (1u << u)] << '\n';
            if (target > (path_cost[v][bit_mask] + graph[v][u])) {
              target = path_cost[v][bit_mask] + graph[v][u];
              //std::cout << "new: " << path_cost[v][bit_mask] + graph[v][u] << '\n';
              prev[u][bit_mask | (1u << u)] = v;
            }
          }
        }
      }
    }
  }

  /*std::cout << "==============\n";
  for (unsigned int vertex = 0; vertex < graph.size(); ++vertex) {
    for (unsigned int bit_mask = 0; bit_mask <= max_bit_mask; ++bit_mask) {
        std::cout << "path_cost[" << vertex << "][" << bit_mask << "] := " << path_cost[vertex][bit_mask] << '\n';
    }
    std::cout << "-------------\n";
  }
  std::cout << "==============\n\n";*/

  unsigned int min_cost     = INF;
  unsigned int final_vertex = INF;

  for (unsigned int vertex = 0; vertex < graph.size(); ++vertex) {
    if (min_cost > path_cost[vertex][max_bit_mask]) {
      min_cost     = path_cost[vertex][max_bit_mask];
      final_vertex = vertex;
    }
  }

  HamiltonianPath result;
  result.min_cost = min_cost;

  unsigned int cur_bit_mask = max_bit_mask;
  unsigned int cur_vertex   = final_vertex;

  do {
    result.vertex_sequence.push_back(cur_vertex + 1);
    cur_bit_mask = cur_bit_mask - (1u << cur_vertex);
    cur_vertex = prev[cur_vertex][cur_bit_mask | (1u << cur_vertex)];
  } while (cur_bit_mask != 0);

  return result;
}

int main() {
  unsigned int vertex_num = 0;
  std::cin >> vertex_num;

  std::vector<std::vector<unsigned int>> graph(vertex_num, std::vector<unsigned int>(vertex_num, 0));

  for (unsigned int i = 0; i < vertex_num; ++i) {
    for (unsigned int j = 0; j < vertex_num; ++j) {
      std::cin >> graph[i][j];
    }
  }

  HamiltonianPath answer = FindHamiltonianPathWithMinCost(graph);

  std::cout << answer.min_cost << '\n';

  for (unsigned int& vertex : answer.vertex_sequence) {
    std::cout << vertex << ' ';
  }
  std::cout << '\n';
  
  return 0;
}