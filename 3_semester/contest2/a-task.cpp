#include <iostream>
#include <vector>
#include <utility>

long long FindMaxNumberOfCorrectTests(const std::vector<std::vector<std::pair<long long, long long>>>& algorithms_array, const long long& max_test_number)
{
    std::vector<std::vector<long long>> ok_test_dp(algorithms_array.size(), std::vector<long long>(max_test_number+1, 0ll));

    for (std::size_t group_index = 1; group_index < algorithms_array.size(); ++group_index)
    {
        for (std::size_t test_num = 1; test_num <= max_test_number; ++test_num)
        {
            for (const std::pair<long long, long long>& pair : algorithms_array[group_index])
            {
                long long all_tests = pair.first;
                long long ok_tests  = pair.second;

                if (all_tests <= test_num) 
                {
                    ok_test_dp[group_index][test_num] = std::max(ok_test_dp[group_index][test_num], ok_test_dp[group_index-1][test_num-all_tests] + ok_tests);
                }
                ok_test_dp[group_index][test_num] = std::max(ok_test_dp[group_index-1][test_num], ok_test_dp[group_index][test_num]);
            }
        }
    }

    return ok_test_dp[algorithms_array.size()-1][max_test_number];
}

int main()
{
    std::size_t algorithms_number   = 0;
    std::size_t groups_number       = 0;
    std::size_t max_tests_number    = 0;

    std::cin >> algorithms_number >> groups_number >> max_tests_number;

    std::vector<std::vector<std::pair<long long, long long>>> algorithms_array(groups_number + 1);

    for (std::size_t i = 0; i < algorithms_number; ++i)
    {
        long long all_tests     = 0;
        long long correct_tests = 0;
        int       group_index   = 0;

        std::cin >> all_tests >> correct_tests >> group_index;
        algorithms_array[group_index].push_back(std::make_pair(all_tests, correct_tests));
    } 

    long long answer = FindMaxNumberOfCorrectTests(algorithms_array, max_tests_number);

    std::cout << answer << '\n';

    return 0;
}