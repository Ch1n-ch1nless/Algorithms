#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <string>

static constexpr int kAlphabetSize = 256;

class SuffixArray {
public:
    explicit SuffixArray(const std::string& str) : copy(str + '\n') {
        std::size_t len = copy.length();
        std::vector<int> transposition(len);
        std::vector<int> classes(len);

        initialSort(transposition, classes);
        iterativeSort(transposition, classes, len);

        indices.assign(transposition.begin() + 1, transposition.end());
    }

    ~SuffixArray() = default;

    std::vector<std::size_t> getIndices() const {
        return indices;
    }

    std::size_t size() const {
        return indices.size();
    }

    std::size_t operator[](std::size_t i) const {
        return indices[i];
    }

private:
    std::vector<size_t> indices;
    std::string copy;

    void initialSort(std::vector<int>& transposition, std::vector<int>& classes) {
        std::size_t len = copy.length();
        std::vector<int> pref_cnt(kAlphabetSize, 0);

        for (std::size_t i = 0; i < len; ++i) {
            ++pref_cnt[copy[i]];
        }
        std::partial_sum(pref_cnt.begin(), pref_cnt.end(), pref_cnt.begin());

        for (std::size_t i = 1; i <= len; ++i) {
            transposition[--pref_cnt[copy[len - i]]] = len - i;
        }

        int class_cnt = 0;
        classes[transposition[0]] = class_cnt;
        for (std::size_t i = 1; i < len; ++i) {
            if (copy[transposition[i]] != copy[transposition[i - 1]]) {
                ++class_cnt;
            }
            classes[transposition[i]] = class_cnt;
        }
    }

    void iterativeSort(std::vector<int>& transposition, std::vector<int>& classes, std::size_t n) {
        std::size_t k = 1;
        while ((1 << k) <= n) {
            std::vector<int> new_transposition(n);
            std::vector<int> new_classes(n);

            sortBySecondHalf(transposition, classes, new_transposition, n, k);
            updateClasses(new_transposition, classes, new_classes, n, k);

            classes = std::move(new_classes);
            transposition = std::move(new_transposition);
            ++k;
        }
    }

    void sortBySecondHalf(const std::vector<int>& transposition, const std::vector<int>& classes,
                         std::vector<int>& new_transposition, std::size_t n, std::size_t k) {
        std::vector<int> pref_cnt(n, 0);
        for (std::size_t i = 0; i < n; ++i) {
            ++pref_cnt[classes[i]];
        }
        std::partial_sum(pref_cnt.begin(), pref_cnt.end(), pref_cnt.begin());

        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            int j = (transposition[i] - (1 << (k - 1)) + n) % n;
            new_transposition[--pref_cnt[classes[j]]] = j;
        }
    }

    void updateClasses(const std::vector<int>& new_transposition, const std::vector<int>& classes,
                      std::vector<int>& new_classes, std::size_t n, std::size_t k) {
        int class_cnt = 0;
        new_classes[new_transposition[0]] = class_cnt;
        for (int i = 1; i < n; ++i) {
            int shift = 1 << (k - 1);
            int curr = (new_transposition[i] + shift) % n;
            int prev = (new_transposition[i - 1] + shift) % n;

            if (classes[curr] != classes[prev] ||
                classes[new_transposition[i]] != classes[new_transposition[i - 1]]) {
                ++class_cnt;
            }
            new_classes[new_transposition[i]] = class_cnt;
        }
    }
};

int main() {
    std::string s;
    std::getline(std::cin, s);

    SuffixArray sa(s);
    auto indices = sa.getIndices();
    for (auto x : indices) {
        std::cout << x + 1 << " ";
    }
    std::cout << "\n";

    return 0;
}