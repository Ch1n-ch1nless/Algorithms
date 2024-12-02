#include <algorithm>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<int>>;

Graph findTransitiveClosureMatrixOfSrcGraph(Graph& src_graph) {
  size_t vertex_number = src_graph.size();

  Graph trans_closure_graph = src_graph;

  for (size_t k = 0; k < vertex_number; ++k) {
    for (size_t i = 0; i < vertex_number; ++i) {
      for (size_t j = 0; j < vertex_number; ++j) {
        if (trans_closure_graph[i][k] == 1 && trans_closure_graph[k][j] == 1) {
          trans_closure_graph[i][j] = 1;
        }
      }
    }
  }

  return trans_closure_graph;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
  size_t vertex_number = 0;
  in >> vertex_number;

  graph.resize(vertex_number);

  for (size_t i = 0; i < vertex_number; i++) {
    graph[i].resize(vertex_number);

    for (size_t j = 0; j < vertex_number; j++) {
      in >> graph[i][j];
    }
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, Graph& graph) {
  for (size_t i = 0; i < graph.size(); ++i) {
    for (size_t j = 0; j < graph[i].size(); ++j) {
      out << graph[i][j] << ' ';
    }
    out << '\n';
  }

  return out;
}

int main() {
  Graph src_graph;

  std::cin >> src_graph;

  Graph answer = findTransitiveClosureMatrixOfSrcGraph(src_graph);

  std::cout << answer;

  return 0;
}