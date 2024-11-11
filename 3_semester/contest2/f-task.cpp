#include <cmath>
#include <iostream>
#include <vector>

using street_t  = int;
using gang_t    = bool;

const bool CONTROVERSIAL_INFLUENCE  = false;
const bool CONSISTENT_INFLUENCE     = true;

bool CheckInfluenceOnSquare(std::size_t street_len, street_t first, street_t second)
{
    for (std::size_t i = 1; i < street_len; ++i)
    {
        gang_t first_prev_home  = (first >> (i-1)) & 1; 
        gang_t first_cur_home   = (first >> i) & 1;
        gang_t second_prev_home = (second >> (i-1)) & 1; 
        gang_t second_cur_home  = (second >> i) & 1;

        if (first_prev_home == first_cur_home && first_cur_home == second_cur_home && second_cur_home == second_prev_home) 
        {
            return CONTROVERSIAL_INFLUENCE;
        }
    }   

    return CONSISTENT_INFLUENCE;
}

long long FindNumberOfOptionsForConsistentDistributionOfInfluence(std::size_t LosSantosWidth, std::size_t LosSantosHeight)
{
    street_t max_street_distr = (1 << LosSantosWidth) - 1;

    std::vector<std::vector<long long>> street_dp(LosSantosHeight + 1, std::vector<long long>(max_street_distr + 1, 0ll));

    for (std::size_t street_distr = 0; street_distr <= max_street_distr; ++street_distr)
    {
        street_dp[1][street_distr] = 1ll;
    }

    for (std::size_t street_index = 2; street_index <= LosSantosHeight; ++street_index)
    {
        for (street_t distr1 = 0; distr1 <= max_street_distr; ++distr1)
        {
            for (street_t distr2 = 0; distr2 <= max_street_distr; ++distr2)
            {
                if (CheckInfluenceOnSquare(LosSantosWidth, distr1, distr2) == CONSISTENT_INFLUENCE)
                {
                    street_dp[street_index][distr2] += street_dp[street_index-1][distr1];
                }
            }
        }
    }

    long long max_number_of_options = 0;

    for (street_t street_distr = 0; street_distr <= max_street_distr; ++street_distr)
    {
        max_number_of_options += street_dp[LosSantosHeight][street_distr];
    }

    return max_number_of_options;
}

int main()
{
    std::size_t LosSantosWidth  = 0;
    std::size_t LosSantosHeight = 0;

    std::cin >> LosSantosHeight >> LosSantosWidth;

    //small optimization
    long long answer = FindNumberOfOptionsForConsistentDistributionOfInfluence( std::min(LosSantosWidth, LosSantosHeight), 
                                                                                std::max(LosSantosHeight, LosSantosWidth)   );

    std::cout << answer << '\n';
}