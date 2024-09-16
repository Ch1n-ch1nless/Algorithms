#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>   

const long long POISON_VALUE = __INT_MAX__;

int BinarySearch(long long* array, int len, long long value) {
    int begin   = 0;
    int end     = len;
    int middle  = 0;

    while (begin + 1 < end) {
        middle = begin + (end - begin) / 2;

        if (array[middle] <= value) {
            end = middle;
        } else {
            begin = middle;
        }
    }

    if (array[begin] >= array[end]) {
        return end;
    }

    return begin;
}

void FindGNIS(long long* array, int n, std::vector<int>& answer) {
    long long* dp       = new long long[n+1];
    long long* pos      = new long long[n+1];
    long long* prev     = new long long[n+1];
    int        length   = 0;

    for (int i = 0; i <= n; i++) {
        dp[i] = -POISON_VALUE;
    }
    dp[0]  = POISON_VALUE;
    pos[0] = -1;

    for (int i = 0; i < n; i++) {
        
        int j = BinarySearch(dp, n, array[i]);

        if (dp[j-1] > array[i]  && array[i] > dp[j]) {
            dp[j] = array[i];
            pos[j] = i;
            prev[i] = pos[j - 1];
            length = std::max(length, j);
        }
    }

    int current_pos = pos[length];

    while (current_pos != -1) {
        answer.push_back(current_pos);
        current_pos = prev[current_pos];
    }
    std::reverse(answer.begin(), answer.end());

    delete[] dp;
    delete[] pos;
    delete[] prev;
}

int main() {
    int n = 0;

    scanf(" %d", &n);

    long long* src_array = new long long[n];

    for (int i = 0; i < n; i++) {
        scanf(" %lld", src_array + i);
    }

    std::vector<int> answer = {};

    FindGNIS(src_array, n, answer);

    printf("%ld\n", answer.size());

    for (int i = 0; i < answer.size(); i++) {
        printf("%d ", answer[i] + 1);
    }
    printf("\n");

    delete[] src_array;

    return 0;
}