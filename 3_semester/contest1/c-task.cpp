#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define BOY  0
#define GIRL 1

bool ComparePairs(const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2);

int  FindMinTimeToDriveAllPeople(std::vector<std::pair<std::string, int>>&  boys_array,
                                 std::vector<std::pair<std::string, int>>&  girls_array,
                                 std::vector<std::vector<std::string>>&     taxis           );

void PrintAllTaxis(std::vector<std::vector<std::string>>& taxis );

int main()
{
    std::size_t max_number_of_boys;
    std::cin >> max_number_of_boys;

    std::vector<std::pair<std::string, int>> boys_array(max_number_of_boys);

    for (std::size_t i = 0; i < max_number_of_boys; ++i)
    {
        std::cin >> boys_array[i].first >> boys_array[i].second;
    }

    std::size_t max_number_of_girls;
    std::cin >> max_number_of_girls;
    
    std::vector<std::pair<std::string, int>> girls_array(max_number_of_girls);

    for (std::size_t i = 0; i < max_number_of_girls; ++i)
    {
        std::cin >> girls_array[i].first >> girls_array[i].second;
    }

    std::vector<std::vector<std::string>> taxis;

    int min_time = FindMinTimeToDriveAllPeople(boys_array, girls_array, taxis);

    std::cout << min_time << '\n';

    PrintAllTaxis(taxis);

    return 0;
}

bool ComparePairs(const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2)
{
    return p1.second < p2.second;
}

int FindMinTimeToDriveAllPeople(std::vector<std::pair<std::string, int>>&  boys_array,
                                std::vector<std::pair<std::string, int>>&  girls_array,
                                std::vector<std::vector<std::string>>&     taxis        )
{
    std::sort(boys_array.begin(),  boys_array.end(),  ComparePairs);
    std::sort(girls_array.begin(), girls_array.end(), ComparePairs);

    std::size_t boys_number  = boys_array.size();
    std::size_t girls_number = girls_array.size();

    std::vector<std::vector<unsigned long long>> dp(boys_number+1, std::vector<unsigned long long>(girls_number+1, __LONG_LONG_MAX__));

    std::vector<std::vector<std::vector<int>>>   prev(boys_number+1, std::vector<std::vector<int>>(girls_number+1, std::vector<int>(2, 0)));

    dp[0][0] = 0;

    for (std::size_t boy_index = 1; boy_index <= boys_number; ++boy_index)
    {
        for (std::size_t girl_index = 0; girl_index <= girls_number; ++girl_index)
        {
            for (std::size_t boys_in_taxi = 1; boys_in_taxi <= 4; ++boys_in_taxi)
            {
                for (std::size_t girls_in_taxi = 0; girls_in_taxi + boys_in_taxi <= 4; ++girls_in_taxi)
                {
                    if (boy_index >= boys_in_taxi && girl_index >= girls_in_taxi)
                    {
                        unsigned long long max_dist = (girl_index == 0)  ? boys_array[boy_index-1].second 
                                                                : std::max(girls_array[girl_index-1].second, boys_array[boy_index-1].second);

                        if (dp[boy_index][girl_index] > dp[boy_index - boys_in_taxi][girl_index - girls_in_taxi] + max_dist)
                        {
                            dp[boy_index][girl_index]           = dp[boy_index - boys_in_taxi][girl_index - girls_in_taxi] + max_dist;
                            prev[boy_index][girl_index][BOY]    = boy_index  - boys_in_taxi;
                            prev[boy_index][girl_index][GIRL]   = girl_index - girls_in_taxi;
                        }
                    }
                }
            }
        }
    }

    //Get answer

    std::vector<std::string> taxi = {};

    int cur_boy  = boys_number;
    int cur_girl = girls_number;

    while (cur_boy != 0 || cur_girl != 0)
    {
        for (int i = cur_boy; i > prev[cur_boy][cur_girl][BOY]; --i)
        {
            taxi.push_back(boys_array[i-1].first);
        }

        for (int i = cur_girl; i > prev[cur_boy][cur_girl][GIRL]; --i)
        {
            taxi.push_back(girls_array[i-1].first);
        }

        taxis.push_back(taxi);

        //Get new values
        int temp_boy_index  = prev[cur_boy][cur_girl][BOY];
        int temp_girl_index = prev[cur_boy][cur_girl][GIRL];

        cur_boy  = temp_boy_index;
        cur_girl = temp_girl_index;
        //----

        taxi.clear();
    } 

    return dp[boys_number][girls_number];  
}

void PrintAllTaxis(std::vector<std::vector<std::string>>& taxis)
{
    std::cout << taxis.size() << '\n';

    for (std::size_t i = 0; i < taxis.size(); ++i)
    {
        std::cout << "Taxi " << i + 1 << ": ";

        int people_num = static_cast<int>(taxis[i].size()) - 1;

        for (int j = 0; j < people_num; ++j)
        {
            std::cout << taxis[i][j] << ((j != people_num-1) ? ", " : " and ");
        }

        std::cout << taxis[i][people_num] << ".\n";
    }
}