#include <algorithm>
#include <cstdio>
#include <vector>

void ReadVector(std::vector<int>& vec, int size)
{
    int temp_var = 0;

    for (int i = 0; i < size; ++i)
    {
        scanf("%d", &temp_var);
        vec.push_back(temp_var);
    }
}

void GetOptimalNumberOfTasks(std::vector<int>& tasks,
                             std::vector<int>& resources,
                             std::vector<int>& awards,
                             int               max_size,
                             int               max_resources )
{
    //Create array
    int** dp = new int*[max_resources + 1];

    for (int i = 0; i <= max_resources; ++i)
    {
        dp[i] = new int[max_size + 1];
    }

    //Clear array
    for (int i = 0; i <= max_resources; ++i)
    {
        for (int j = 0; j <= max_size; ++j)
        {
            dp[i][j] = 0;
        }
    }

    //Calculate
    for (int i = 1; i <= max_resources; ++i)
    {
        for (int j = 1;j <= max_size; ++j)
        {
            dp[i][j] = dp[i][j-1];

            if (i >= resources[j-1])
            {
                dp[i][j] = std::max(dp[i][j], dp[i-resources[j-1]][j-1] + awards[j-1]);
            }
        }
    }

    //Get answer
    int max_award = dp[max_resources][max_size];

    int i = max_resources;
    int j = max_size;

    while (dp[i][j] > 0)
    {
        if (dp[i][j-1] == max_award)
        {
            --j;
        }
        else
        {
            tasks.push_back(j);
            --j;
            i -= resources[j];
            max_award = dp[i][j];
        }
    }

    std::reverse(tasks.begin(), tasks.end());

    //Destroy array
    for (int i = 0; i <= max_resources; ++i)
    {
        delete[] dp[i];
    }

    delete dp;
}

int main()
{
    int number_of_tasks = 0;
    int cj_number_res   = 0;

    scanf("%d %d", &number_of_tasks, &cj_number_res);

    std::vector<int> resources = {};
    std::vector<int> awards    = {};

    ReadVector(resources, number_of_tasks);
    ReadVector(awards,    number_of_tasks);

    std::vector<int> tasks = {};

    GetOptimalNumberOfTasks(tasks, resources, awards, number_of_tasks, cj_number_res);

    for (int& task_index : tasks)
    {
        printf("%d\n", task_index);
    }

    return 0;
}