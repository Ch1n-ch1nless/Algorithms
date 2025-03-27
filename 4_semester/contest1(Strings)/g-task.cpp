#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <utility>

class CyclicPatternOccurrencesCounter {
private:
    // Первый хэш (основной)
    static constexpr std::size_t base1_ = 53;
    static constexpr std::size_t mod1_ = static_cast<std::size_t>(1e9 + 7);
    
    // Второй хэш (дополнительный)
    static constexpr std::size_t base2_ = 911;
    static constexpr std::size_t mod2_ = static_cast<std::size_t>(1e9 + 9);

    std::vector<std::size_t> pows1_, pows2_;
    std::unordered_set<std::size_t> cyclic_pattern_hashes1;
    std::unordered_set<std::size_t> cyclic_pattern_hashes2;
    
    std::vector<std::size_t> pref_text_hashes1, pref_text_hashes2;
    std::vector<std::size_t> pref_pattern_hashes1, pref_pattern_hashes2;

    std::size_t mul(std::size_t lhs, std::size_t rhs, const std::size_t& mod) const {
      uint64_t res = static_cast<uint64_t>(lhs) * static_cast<uint64_t>(rhs);
      res = res % (static_cast<uint64_t>(mod));
      return static_cast<std::size_t>(res);
    }

    std::size_t add(std::size_t lhs, std::size_t rhs, const std::size_t& mod) const {
      uint64_t res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(rhs);
      res = res % (static_cast<uint64_t>(mod));
      return static_cast<std::size_t>(res);
    }

    std::size_t sub(std::size_t lhs, std::size_t rhs, const std::size_t& mod) const {
      uint64_t res = static_cast<uint64_t>(lhs) + static_cast<uint64_t>(mod);
      res = res - static_cast<uint64_t>(rhs);
      res = res % (static_cast<uint64_t>(mod));
      return static_cast<std::size_t>(res);
    }

    void computePows(std::size_t max_len) {
        pows1_.resize(max_len + 1);
        pows2_.resize(max_len + 1);
        
        pows1_[0] = 1;
        pows2_[0] = 1;
        
        for (std::size_t i = 1; i <= max_len; ++i) {
            pows1_[i] = mul(pows1_[i - 1], base1_, mod1_);
            pows2_[i] = mul(pows2_[i - 1], base2_, mod2_);
        }
    }

    std::pair<std::size_t, std::size_t> getSubstringHash(
        const std::vector<std::size_t>& pref1,
        const std::vector<std::size_t>& pref2,
        std::size_t begin, std::size_t end) const 
    {
        std::size_t len = end - begin;
        
        std::size_t hash1 = sub(pref1[end], mul(pref1[begin], pows1_[len], mod1_), mod1_);
        std::size_t hash2 = sub(pref2[end], mul(pref2[begin], pows2_[len], mod2_), mod2_);
        
        return {hash1, hash2};
    }

    void fillCyclicPatternHashes() {
        std::size_t pattern_len = (pref_pattern_hashes1.size() - 1) / 2;
        
        for (std::size_t i = 0; i < pattern_len; ++i) {
            auto [hash1, hash2] = getSubstringHash(
                pref_pattern_hashes1, pref_pattern_hashes2, 
                i, i + pattern_len
            );
            
            cyclic_pattern_hashes1.insert(hash1);
            cyclic_pattern_hashes2.insert(hash2);
        }
    }

    void fillPrefixArray(
        std::vector<std::size_t>& pref1,
        std::vector<std::size_t>& pref2,
        const std::string& str) 
    {
        pref1.resize(str.size() + 1);
        pref2.resize(str.size() + 1);
        
        pref1[0] = pref2[0] = 0;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            pref1[i+1] = add(mul(pref1[i], base1_, mod1_), str[i] - 'a', mod1_);
            pref2[i+1] = add(mul(pref2[i], base2_, mod2_), str[i] - 'a', mod2_);
        }
    }

public:
    CyclicPatternOccurrencesCounter(const std::string& text,
                                  const std::string& pattern) 
    {
        std::size_t max_len = std::max(text.size(), pattern.size() * 2);
        computePows(max_len);

        fillPrefixArray(pref_text_hashes1, pref_text_hashes2, text);
        
        std::string doubled_pattern = pattern + pattern;
        fillPrefixArray(pref_pattern_hashes1, pref_pattern_hashes2, doubled_pattern);

        fillCyclicPatternHashes();
    }

    std::size_t operator()() {
        std::size_t text_len = pref_text_hashes1.size() - 1;
        std::size_t pattern_len = (pref_pattern_hashes1.size() - 1) / 2;

        if (text_len < pattern_len) {
            return 0;
        }

        std::size_t count = 0;
        for (std::size_t i = 0; i <= text_len - pattern_len; ++i) {
            auto [hash1, hash2] = getSubstringHash(
                pref_text_hashes1, pref_text_hashes2,
                i, i + pattern_len
            );
            
            if (cyclic_pattern_hashes1.count(hash1) && 
                cyclic_pattern_hashes2.count(hash2)) 
            {
                ++count;
            }
        }

        return count;
    }
};

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;

    CyclicPatternOccurrencesCounter counter(text, pattern);
    std::cout << counter() << "\n";

    return 0;
}