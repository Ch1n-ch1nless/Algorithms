#include <algorithm>
#include <iostream>
#include <vector>

using Vertex = int;

class Graph {
public:
  Graph() = default;
  Graph(size_t vertex_number);
  Graph(const Graph& other);

  ~Graph() = default;

  Graph& operator=(const Graph& other);

  std::vector<bool>& operator[](const Vertex& vertex);
  const std::vector<bool>& operator[](const Vertex& vertex) const;

  size_t getVertexNum() const;
  void   setVertexNum(size_t new_vertex_number);

  void addEdge(const Vertex& start, const Vertex& end);
  bool isEdgeExists(const Vertex& start, const Vertex& end) const;

private:
  std::vector<std::vector<bool>> adjacency_matrix_;
};

Graph::Graph(size_t vertex_number) 
:
  adjacency_matrix_(vertex_number, std::vector<bool>(vertex_number, false))
{
}

Graph::Graph(const Graph& other)
:
  adjacency_matrix_(other.adjacency_matrix_)
{
}

Graph& Graph::operator=(const Graph& other) {
  if (this != &other) {
    adjacency_matrix_ = other.adjacency_matrix_;
  }
  return *this;
}

std::vector<bool>& Graph::operator[](const Vertex& vertex) {
  return adjacency_matrix_[vertex];
}

const std::vector<bool>& Graph::operator[](const Vertex& vertex) const {
  return adjacency_matrix_[vertex];
}

size_t Graph::getVertexNum() const {
  return adjacency_matrix_.size();
}

void Graph::setVertexNum(size_t new_vertex_number) {
  adjacency_matrix_.resize(new_vertex_number);
  for (auto iter = adjacency_matrix_.begin(); iter != adjacency_matrix_.end(); ++iter) {
    iter->resize(new_vertex_number);
  }
}

void Graph::addEdge(const Vertex& start, const Vertex& end) {
  adjacency_matrix_[start][end] = true;
}

bool Graph::isEdgeExists(const Vertex& start, const Vertex& end) const {
  return adjacency_matrix_[start][end];
}

Graph findTransitiveClosureMatrixOfSrcGraph(const Graph& src_graph) {
  size_t vertex_number = src_graph.getVertexNum();

  Graph trans_closure_graph = src_graph;

  for (size_t k = 0; k < vertex_number; ++k) {
    for (size_t i = 0; i < vertex_number; ++i) {
      for (size_t j = 0; j < vertex_number; ++j) {
        if (trans_closure_graph.isEdgeExists(i, k) && trans_closure_graph.isEdgeExists(k, j)) {
          trans_closure_graph.addEdge(i, j);
        }
      }
    }
  }

  return trans_closure_graph;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
  size_t vertex_number = 0;
  in >> vertex_number;

  bool is_edge_exist = false;

  graph.setVertexNum(vertex_number);
  for (size_t i = 0; i < vertex_number; i++) {
    for (size_t j = 0; j < vertex_number; j++) {
      in >> is_edge_exist;
      if (is_edge_exist) graph.addEdge(i, j);
    }
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  for (size_t i = 0; i < graph.getVertexNum(); ++i) {
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