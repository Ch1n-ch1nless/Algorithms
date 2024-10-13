#include <cassert>
#include <iostream>
#include <vector>

const int MODULE = 1000000007;

bool IsMaskFitTemplate(int mask, int bit_number, const std::vector<char>& district)
{
    for (int i = 0; i < bit_number; i++)
    {
        if (((mask & 1) == 1) && (district[i] == '-'))
        {
            return false;
        } 
        if (((mask & 1) == 0) && (district[i] == '+'))
        {
            return false;
        }

        mask >>= 1;
    }

    return true;
}

bool BuildBitMask(const int& mask_size, int& new_mask, int old_mask)
{
    for (int i = 1; i < mask_size; ++i)
    {
        int sum = ((old_mask >> i) & 1) + ((old_mask >> (i-1)) & 1) + ((new_mask >> (i-1)) & 1);
        
        if (sum == 0 || sum == 3) return false;
    
        if (sum == 1) new_mask += (1 << i);
    }

    return true;
}

int FindNumberOfDifferentPartitions(const int& width, const int& height, const std::vector<std::vector<char>>& districts)
{
    int max_bit_mask = 1 << width;

    std::vector<std::vector<int>> dp(height, std::vector<int>(max_bit_mask, 0));

    for (int bit_mask = 0; bit_mask < max_bit_mask; ++bit_mask)
    {
        if (IsMaskFitTemplate(bit_mask, width, districts[0]))
        {
            dp[0][bit_mask] = 1;
        }
    }

    for (int h = 1; h < height; ++h)
    {
        for (int prev_mask = 0; prev_mask < max_bit_mask; ++prev_mask)
        {
            if (IsMaskFitTemplate(prev_mask, width, districts[h-1]))
            {

                int cur_mask = 1;

                bool is_impossible = BuildBitMask(width, cur_mask, prev_mask);

                if (is_impossible && IsMaskFitTemplate(cur_mask, width, districts[h]))
                {
                    dp[h][cur_mask] = (dp[h][cur_mask] + dp[h-1][prev_mask]) % MODULE;
                }

                cur_mask = 0;

                is_impossible = BuildBitMask(width, cur_mask, prev_mask);

                if (is_impossible && IsMaskFitTemplate(cur_mask, width, districts[h]))
                {
                    dp[h][cur_mask] = (dp[h][cur_mask] + dp[h-1][prev_mask]) % MODULE;
                }
            }
        }
    }

    int answer = 0;

    for (int bit_mask = 0; bit_mask < max_bit_mask; ++bit_mask)
    {
        answer = (answer + dp[height-1][bit_mask]) % MODULE;
    }

    return answer;
}   

std::vector<std::vector<char>> ReverseArray(int& width, int& height, const std::vector<std::vector<char>>& districts)
{
    std::vector<std::vector<char>> new_districts(width, std::vector<char>(height));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            new_districts[i][j] = districts[j][i];
        }
    }

    int temp_var = height;
    height = width;
    width = temp_var;

    return new_districts;
}

int main()
{
    int height  = 0;
    int width   = 0;

    std::cin >> height >> width;

    std::vector<std::vector<char>> districts(height, std::vector<char>(width));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            std::cin >> districts[i][j];
        }
    }

    if (width > height)
    {
        districts = ReverseArray(width, height, districts);
    }

    int answer = FindNumberOfDifferentPartitions(width, height, districts);

    std::cout << answer << '\n';

    return 0;
}