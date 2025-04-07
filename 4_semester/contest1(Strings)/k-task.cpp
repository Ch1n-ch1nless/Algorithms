#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

template <typename SymbolType, class SymbolContainer>
class Trie {
 public:
  //-------------------------------------------------------------------
  // Type aliases
  //-------------------------------------------------------------------
  using char_t = SymbolType;
  using string_t = SymbolContainer;
  using index_t = int64_t;

  //-------------------------------------------------------------------
  // Constants
  //-------------------------------------------------------------------
  static constexpr index_t kBeforeRootId = -1;
  static constexpr index_t kRootId = 0;

  struct Node {
    Node() = default;
    Node(index_t parent, char_t symbol)
        : children{}, parent(parent), symbol(symbol), is_terminal{false} {}

    bool hasChild(char_t symbol) const {
        return static_cast<bool>(children.count(symbol));
    }

    std::unordered_map<char_t, index_t> children;
    index_t parent{kBeforeRootId};
    char_t symbol{};
    bool is_terminal{false};
  };

  //-------------------------------------------------------------------
  // Constructor
  //-------------------------------------------------------------------
  Trie() : nodes_(1) {}

  //-------------------------------------------------------------------
  // Public methods
  //-------------------------------------------------------------------
  index_t add(const string_t& symbol_container) {
    index_t current_node = kRootId;

    for (auto symbol : symbol_container) {
      if (!nodes_[current_node].hasChild(symbol)) {
        nodes_.emplace_back(current_node, symbol);
        nodes_[current_node].children[symbol] = nodes_.size() - 1;
      }
      current_node = nodes_[current_node].children[symbol];
    }

    nodes_[current_node].is_terminal = true;
    return current_node;
  }

  Node& get(const index_t& index) { return nodes_[index]; }
  const Node& get(const index_t& index) const { return nodes_[index]; }

  std::size_t size() const { return nodes_.size(); }

 private:
  //-------------------------------------------------------------------
  // Data
  //-------------------------------------------------------------------
  std::vector<Node> nodes_;
};

template <typename SymbolType, class SymbolContainer>
class AhoKorasikAutomation {
 public:
  //-------------------------------------------------------------------
  // Type aliases
  //-------------------------------------------------------------------
  using trie_t = Trie<SymbolType, SymbolContainer>;
  using char_t = typename trie_t::char_t;
  using string_t = typename trie_t::string_t;
  using index_t = typename trie_t::index_t;

  //-------------------------------------------------------------------
  // Public methods
  //-------------------------------------------------------------------
  void add(const string_t& symbol_container) {
    trie_.add(symbol_container);
  }

  void buildAutomaton() {
    const index_t root = trie_t::kRootId;
    failure_links_.assign(trie_.size(), trie_t::kBeforeRootId);

    std::queue<index_t> queue;

    for (const auto& pair : trie_.get(root).children) {
      index_t child_idx = pair.second;
      failure_links_[child_idx] = root;
      queue.push(child_idx);
    }

    while (!queue.empty()) {
      index_t current_idx = queue.front();
      queue.pop();

      for (const auto& pair : trie_.get(current_idx).children) {
        char_t symbol = pair.first;
        index_t child_idx = pair.second;
        queue.push(child_idx);

        index_t failure = failure_links_[current_idx];
        while (failure != trie_t::kBeforeRootId && !trie_.get(failure).hasChild(symbol)) {
          failure = failure_links_[failure];
        }

        failure_links_[child_idx] = (failure == trie_t::kBeforeRootId)
                                        ? root
                                        : trie_.get(failure).children.at(symbol);
      }
    }
  }

  std::vector<index_t> findMatches(const string_t& text) {
    std::vector<index_t> matches;
    index_t current_state = trie_t::kRootId;

    for (auto symbol : text) {
      while (true) {
        const auto& current_node = trie_.get(current_state);
        if (current_node.hasChild(symbol)) {
          current_state = current_node.children.at(symbol);
          break;
        } else {
          if (current_state == trie_t::kRootId) break;
          current_state = failure_links_[current_state];
        }
      }

      index_t temp = current_state;
      while (temp != trie_t::kBeforeRootId) {
        if (trie_.get(temp).is_terminal) {
          matches.push_back(temp);
        }
        temp = failure_links_[temp];
      }
    }

    return matches;
  }

 private:
  trie_t trie_;
  std::vector<index_t> failure_links_;
};

int main() {
  std::string pattern, text;
  std::cin >> pattern >> text;

  AhoKorasikAutomation<char, std::string> automaton;
  automaton.add(text);
  automaton.buildAutomaton();

  auto matches = automaton.findMatches(pattern);

  for (auto idx : matches) {
    std::cout << idx << " ";
  }
  std::cout << "\n";

  return 0;
}