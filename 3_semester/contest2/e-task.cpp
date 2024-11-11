#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

bool IsComputerInMask(unsigned int bit_mask, unsigned int computer) {
  unsigned int computer_mask = 1u << computer;
  return ((bit_mask & computer_mask) == computer_mask) ? true : false;
}

int FindMaxComputerNumber(const int& computer_number, const std::vector<std::vector<bool>>& isComputersConnected)
{
    int max_bit_mask = (1 << computer_number) - 1;
    std::vector<int> dp(max_bit_mask + 1, 0);

    for (int bit_mask = 1; bit_mask <= max_bit_mask; ++bit_mask)
    {
        for (int i = 0; i < computer_number; ++i)
        {
            if (IsComputerInMask(bit_mask, i))
            {
                for (int j = 0; j < computer_number; j++)
                {
                    if (IsComputerInMask(bit_mask, j) && i != j && isComputersConnected[i][j])
                    {
                        int old_bit_mask = bit_mask - (1 << i) - (1 << j);
                        dp[bit_mask] = std::max(dp[old_bit_mask] + 2, dp[bit_mask]);
                    }
                }
            }
        }
    }

    return dp[max_bit_mask];
}

int main()
{
    int max_comp_num = 0;
    std::cin >> max_comp_num;

    std::vector<std::vector<bool>> isComputerConnected(max_comp_num, std::vector<bool>(max_comp_num));

    for (std::size_t i = 0; i < max_comp_num; ++i)
    {
        for (std::size_t j = 0; j < max_comp_num; ++j)
        {
            char c;
            std::cin >> c;

            isComputerConnected[i][j] = (c == 'Y') ? true : false;
        }
    }

    int answer = FindMaxComputerNumber(max_comp_num, isComputerConnected);

    std::cout << answer << '\n';
}