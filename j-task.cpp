#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int FindLargestCommonSequence(std::string&      cj_dance, std::string&      caesar_dance, 
                              std::vector<int>& cj_index, std::vector<int>& caesar_index  ) 
{
    int cj_len      = cj_dance.size();
    int caesar_len  = caesar_dance.size();

    int answer      = 0;

    int i_begin     = 0;
    int j_begin     = 0;

    int dp[1001][1001] = {};

    for (int i = 0; i <= cj_len; ++i) {
        for (int j = 0; j <= caesar_len; ++j) {
            dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= cj_len; ++i) {
        for (int j = 1; j <= caesar_len; ++j) {
            
            dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);

            if (cj_dance[i-1] == caesar_dance[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            }

            if (dp[i][j] > answer) {
                answer = dp[i][j];
                i_begin = i;
                j_begin = j;
            }
        }
    }

    int count = answer;

    while (count != 0) {

        if (cj_dance[i_begin-1] == caesar_dance[j_begin-1]) {
            cj_index.push_back(i_begin);
            caesar_index.push_back(j_begin);

            i_begin--;
            j_begin--;
            count--;
        } else {
            if (dp[i_begin-1][j_begin] < dp[i_begin][j_begin-1]) {
                j_begin--;
            } else {
                i_begin--;
            }
        }
    }

    std::reverse(cj_index.begin(), cj_index.end());
    std::reverse(caesar_index.begin(), caesar_index.end());

    return answer;
}

int main() {
    std::string cj_dance        = {};
    std::string caesar_dance    = {};

    std::cin >> cj_dance;
    std::cin >> caesar_dance;

    std::vector<int> cj_index       = {};
    std::vector<int> caesar_index   = {};

    int answer = FindLargestCommonSequence(cj_dance, caesar_dance, cj_index, caesar_index);

    std::cout << answer << '\n';

    for (int& index : cj_index) {
        std::cout << index << ' ';
    }
    std::cout << '\n';

    for (int& index : caesar_index) {
        std::cout << index << ' ';
    }
    std::cout << '\n';

    return 0;
}