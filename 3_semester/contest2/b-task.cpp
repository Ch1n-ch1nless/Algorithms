#include <cassert>
#include <iostream>
#include <string>
#include <vector>

const int IMPOSSIBLE_TRANSFORMATION = -1;

int FindNumberOfDifferentIndexes(const std::string& alpha, const std::string& beta, const unsigned int max_transform_num)
{
    if (abs(alpha.size() - beta.size()) > max_transform_num) return IMPOSSIBLE_TRANSFORMATION;

    std::vector<std::vector<std::vector<int>>> dp(beta.size() + 1, std::vector<std::vector<int>>(max_transform_num + 1, std::vector<int>(max_transform_num + 1, 0)));

    for (std::size_t len = 1; len <= beta.size(); ++len)
    {
        for (unsigned int i = 0; i <= max_transform_num; ++i)
        {
            for (unsigned int j = 0; j <= max_transform_num; ++j)
            {
                if (i != 0) 
                {
                    dp[len][i][j] = std::max(dp[len][i][j], dp[len][i-1][j]);       //No changes
                }

                if (j != 0)
                {
                    dp[len][i][j] = std::max(dp[len][i][j], dp[len-1][i][j-1] + 1); //Add letter
                }

                if ((len + i > j) && ((len + i - j) <= alpha.size()))
                {
                    int indicator = (alpha[len+i-j-1] == beta[len-1]);
                    dp[len][i][j] = std::max(dp[len][i][j], dp[len-1][i][j] + indicator);
                }
            }
        }
    }

    int result = 0;

    for (unsigned int i = 0; i <= max_transform_num; ++i)
    {
        for (unsigned int j = 0; i + j <= max_transform_num; ++j)
        {
            if (alpha.size() + j == beta.size() + i)
            {
                result = std::max(result, dp[beta.size()][i][j] + (int)(max_transform_num - i - j));
            }
        }
    }

    result = std::max(0, (int)beta.size() - result);
 
    return result;
}

int main()
{
    std::string alpha;
    std::string beta;

    unsigned int max_transform_number = 0;

    std::cin >> alpha >> beta >> max_transform_number;

    int answer = FindNumberOfDifferentIndexes(alpha, beta, max_transform_number);

    std::cout << answer << '\n';

    return 0;
}