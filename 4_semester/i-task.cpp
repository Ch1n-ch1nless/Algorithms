#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

#define MOD_VALUE 7340033
#define ROOT_VALUE 5
#define ROOT_POWER 1048576

class MathOperations {
public:
    static int Sum(int a, int b) { return (a + b) % MOD_VALUE; }

    static int Diff(int a, int b) { return (a - b + MOD_VALUE) % MOD_VALUE; }

    static int Product(int a, int b) { return (static_cast<long long>(a) * b) % MOD_VALUE; }

    static int FastPow(int base, int exp) {
        int res = 1;
        while (exp > 0) {
            if (exp & 1) {
                res = Product(res, base);
            }
            base = Product(base, base);
            exp >>= 1;
        }
        return res;
    }

    static int GetInverse(int val) { return FastPow(val, MOD_VALUE - 2); }
};

void Transform(std::vector<int>& data, bool invert) {
    int n = data.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(data[i], data[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        int w = invert ? MathOperations::GetInverse(ROOT_VALUE) : ROOT_VALUE;
        for (int i = len; i < ROOT_POWER; i <<= 1) {
            w = MathOperations::Product(w, w);
        }

        for (int i = 0; i < n; i += len) {
            int w_current = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = data[i + j];
                int v = MathOperations::Product(data[i + j + len / 2], w_current);
                data[i + j] = MathOperations::Sum(u, v);
                data[i + j + len / 2] = MathOperations::Diff(u, v);
                w_current = MathOperations::Product(w_current, w);
            }
        }
    }

    if (invert) {
        int inv_n = MathOperations::GetInverse(n);
        for (int& x : data) {
            x = MathOperations::Product(x, inv_n);
        }
    }
}

std::vector<int> PolyMultiply(std::vector<int> const& p1, std::vector<int> const& p2) {
    std::vector<int> a(p1.begin(), p1.end());
    std::vector<int> b(p2.begin(), p2.end());
    int size = 1;
    while (size < static_cast<int>(p1.size() + p2.size())) {
        size <<= 1;
    }
    a.resize(size);
    b.resize(size);

    Transform(a, false);
    Transform(b, false);
    for (int i = 0; i < size; ++i) {
        a[i] = MathOperations::Product(a[i], b[i]);
    }
    Transform(a, true);

    return a;
}

std::vector<int> ComputeInverse(std::vector<int> poly, int current_mod) {
    if (current_mod == 1) {
        if (poly[0] == 0) {
            return {INT_MAX};
        }
        return {MathOperations::GetInverse(poly[0])};
    }

    auto prev_result = ComputeInverse(poly, current_mod / 2);
    
    std::vector<int> lower_half(current_mod / 2);
    std::copy(poly.begin(), poly.begin() + current_mod / 2, lower_half.begin());
    
    std::vector<int> upper_half(current_mod / 2);
    std::copy(poly.begin() + current_mod / 2, poly.begin() + current_mod, upper_half.begin());

    auto first_product = PolyMultiply(lower_half, prev_result);
    std::vector<int> remaining_terms;
    for (int i = current_mod / 2; i < static_cast<int>(first_product.size()); ++i) {
        remaining_terms.push_back(first_product[i]);
    }

    auto second_product = PolyMultiply(prev_result, remaining_terms);
    auto third_product = PolyMultiply(upper_half, prev_result);
    third_product = PolyMultiply(third_product, prev_result);

    int max_size = std::max(second_product.size(), third_product.size());
    second_product.resize(max_size);
    third_product.resize(max_size);
    std::vector<int> combined_terms(max_size);
    for (int i = 0; i < max_size; ++i) {
        combined_terms[i] = MathOperations::Diff(0, MathOperations::Sum(second_product[i], third_product[i]));
    }

    std::vector<int> result(current_mod);
    std::copy(prev_result.begin(), prev_result.end(), result.begin());
    for (int i = current_mod / 2; i < current_mod; ++i) {
        result[i] = combined_terms[i - current_mod / 2];
    }
    return result;
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::vector<int> coefficients(n + 1);
    for (int i = 0; i <= n; ++i) {
        std::cin >> coefficients[i];
    }

    int mod = 1;
    while (mod < m) {
        mod <<= 1;
    }
    coefficients.resize(std::max(static_cast<int>(coefficients.size()), mod));

    auto inverse = ComputeInverse(coefficients, mod);
    if (inverse[0] == INT_MAX) {
        std::cout << "The ears of a dead donkey\n";
        return 0;
    }
    for (int i = 0; i < m; ++i) {
        std::cout << inverse[i] << ' ';
    }
    return 0;
}