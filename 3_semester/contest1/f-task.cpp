#include <cassert>
#include <iostream>
#include <vector>

const int PREVIOUS = 0;
const int CURRENT  = 1;

int FindMinNumberOfExperiments(const int &max_height, const int &planes_number);

int main()
{
    int max_height          = 0;
    int number_of_planes    = 0;

    std::cin >> max_height >> number_of_planes;

    int answer = (max_height <= 2) ? std::max(0, max_height - 1) : FindMinNumberOfExperiments(max_height, number_of_planes);

    std::cout << answer << '\n';

    return 0;
}

int FindMinNumberOfExperiments(const int &max_height, const int &planes_number)
{
    std::vector<std::vector<int>> dp(2, std::vector<int>(planes_number + 1, 1));

    for (int i = 1; i <= planes_number; ++i)
    {
        dp[PREVIOUS][i] = 2;
    }

    for (int i = 2; i <= max_height; ++i)
    {
        dp[CURRENT][0] = 1;
        dp[CURRENT][1] = i+1;

        for (int j = 2; j <= planes_number; ++j)
        {
            dp[CURRENT][j] = dp[PREVIOUS][j-1] + dp[PREVIOUS][j];
        }

        if (dp[CURRENT][planes_number] >= max_height)
        {
            return i;
        }

        for (int i = 0; i < planes_number; ++i)
        {
            dp[PREVIOUS][i] = dp[CURRENT][i];
        }
    }

    return -1;
}