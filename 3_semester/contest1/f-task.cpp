#include <cmath>
#include <iostream>
#include <vector>

int FindMinNumberOfExperiments(const std::size_t& max_height, const std::size_t& number_of_planes);

int main()
{
    std::size_t max_height       = 0;
    std::size_t number_of_planes = 0;

    std::cin >> max_height >> number_of_planes;

    int answer = FindMinNumberOfExperiments(max_height, number_of_planes);

    std::cout << answer << '\n';

    return 0;
}

int CheckWithoutDp(const std::size_t& k, const std::size_t& n)
{
    std::size_t value = 1;
    
    for (std::size_t pow = 0; pow <= k; ++pow)
    {
        if (value >=  n)
        {
            return pow;
        }

        value *= 2;
    }

    return -1;
}

int FindMinNumberOfExperiments(const std::size_t& max_height, const std::size_t& number_of_planes)
{
    if (number_of_planes == 0)
    {
        return (max_height == 1) ? 0 : -1;
    }

    int answer = CheckWithoutDp(number_of_planes, max_height);

    if (answer == -1)
    {
        std::vector<std::vector<int>> dp(max_height+1, std::vector(number_of_planes + 1, 1));

        for (std::size_t try_num = 1; try_num <= max_height; ++try_num)
        {
            for (std::size_t plane_index = 1; plane_index <= number_of_planes; ++plane_index)
            {
                dp[try_num][plane_index] = dp[try_num-1][plane_index-1] + dp[try_num-1][plane_index];
            }

            if (dp[try_num][number_of_planes] >= max_height)
            {
                return try_num;
            }
        }
    }   

    return answer;
}