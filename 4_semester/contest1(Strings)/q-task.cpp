#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class SuffixAutomaton {
 private:
  static constexpr int64_t kBeforeRoot = -1;
  static constexpr int64_t kRootId = 0;

 public:
  struct State {
    std::map<char, int64_t> transitions_;
    int64_t longest_;
    int64_t suffix_link_;
  };

  SuffixAutomaton(const std::string& str) {
    createNewState(0, kBeforeRoot);
    last_state_id_ = kRootId;
    count = 0;
    for (auto& symbol : str) {
      addSymbol(symbol);
    }
  }

  SuffixAutomaton() {
    createNewState(0, kBeforeRoot);
    last_state_id_ = kRootId;
    count = 0;
  }

  void addSymbol(const char& symbol) {
    int64_t new_state_id = createNewState(states_[last_state_id_].longest_ + 1, kBeforeRoot);

    int64_t current_id = last_state_id_;
    while (current_id != kBeforeRoot && !hasTransition(current_id, symbol)) {
      createTransition(current_id, new_state_id, symbol);
      current_id = getSuffixLink(current_id);
    }

    if (current_id == kBeforeRoot) {
      states_[new_state_id].suffix_link_ = kRootId;
    } else {
      int64_t next_state_id = states_[current_id].transitions_[symbol];
      if (states_[next_state_id].longest_ == states_[current_id].longest_ + 1) {
        states_[new_state_id].suffix_link_ = next_state_id;
      } else {
        int64_t clone_id = cloneState(
          next_state_id,
          states_[current_id].longest_ + 1
        );

        while (current_id != kBeforeRoot && 
                  states_[current_id].transitions_[symbol] == next_state_id) {
          states_[current_id].transitions_[symbol] = clone_id;
          current_id = getSuffixLink(current_id);
        }

        states_[next_state_id].suffix_link_ = clone_id;
        states_[new_state_id].suffix_link_ = clone_id;
      }
    }

    last_state_id_ = new_state_id;
    count += countEqualentStrings(new_state_id);
    unique_numbers_.push_back(count);
  }

  const std::vector<int64_t>&
  getUniqueSubstringsNumbers() {
    return unique_numbers_;
  }

  const std::vector<State>& getStates() {
    return states_;
  }

  const int64_t getLastStateId() {
    return last_state_id_;
  }

 private:
  int64_t createNewState(const int64_t& longest, const int64_t& suffix_link) {
    int64_t index = static_cast<int64_t>(states_.size());
    states_.push_back(State{{}, longest, suffix_link});
    return index;
  }

  bool hasTransition(const int64_t& state_id, const char& symbol) {
    return static_cast<bool>(states_[state_id].transitions_.count(symbol));
  }

  void createTransition(const int64_t& parent_id, const int64_t& child_id, const char& symbol) {
    states_[parent_id].transitions_[symbol] = child_id;
  }

  int64_t getSuffixLink(const int64_t& state_id) {
    return states_[state_id].suffix_link_; 
  }

  int64_t cloneState(const int64_t& src_state_id, const int64_t& new_length) {
    State& src = states_[src_state_id];
    int64_t clone_id = createNewState(new_length, src.suffix_link_);

    states_[clone_id].transitions_ = states_[src_state_id].transitions_;

    return clone_id;
  }

  int64_t countEqualentStrings(int64_t state_id) {
    int64_t link = states_[state_id].suffix_link_;
    return states_[state_id].longest_ - states_[link].longest_;
  }

  std::vector<State> states_;
  std::vector<int64_t> unique_numbers_;
  int64_t last_state_id_;
  int64_t count;
};

int main() {
  std::string s = "";
  std::cin >> s;

  SuffixAutomaton suffix_automation(s);

  auto answer = suffix_automation.getUniqueSubstringsNumbers();

  for (auto& number : answer) {
    std::cout << number << "\n";
  }

  return 0;
}