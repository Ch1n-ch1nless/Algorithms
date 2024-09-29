#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

void FindDistancesBetweenPoints (std::vector<int> &coords, std::vector<std::vector<std::vector<int>>> &dist, int n);
int  FindPointsOfInfluence      (std::vector<int> &coords, std::vector<int> &positions);
int  BinarySearch               (std::vector<int> &coords, int pivot);

int main()
{
    int n = 0;
    int m = 0;

    std::cin >> n >> m;

    std::vector<int> coords(n);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> coords[i];
    }

    std::vector<int> positions(m, -1);

    int min_len = FindPointsOfInfluence(coords, positions);

    std::cout << min_len << '\n';

    for (int i = 0; i < m; ++i)
    {
        std::cout << positions[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}

int FindPointsOfInfluence(std::vector<int> &coords, std::vector<int> &positions)
{
    int n = static_cast<int>(coords.size());
    int m = static_cast<int>(positions.size());

    std::vector< std::vector< std::vector <int> > > dist(n, std::vector< std::vector<int> >(n, std::vector<int>(2, 0)));

    FindDistancesBetweenPoints(coords, dist, n);

    std::vector<std::vector<int>> dp  (m, std::vector<int>(n, 0));
    std::vector<std::vector<int>> prev(m, std::vector<int>(n, 0));

    for (int j = 0; j < n; ++j)
    {
        dp[0][j] = dist[j][0][1];
    }

    for (int i = 1; i < m; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            dp[i][j] = dp[0][n-1];

            for (int k = i-1; k < j; ++k) 
            {
                int mid_pos = BinarySearch(coords, (coords[j] + coords[k] + 1) / 2);
                
                int cur_len = dp[i-1][k] + dist[k][mid_pos-1][0] + dist[j][mid_pos][1];

                if (cur_len < dp[i][j]) 
                {
                    dp  [i][j]  = cur_len;
                    prev[i][j]  = k;
                }
            }
        }
    }

    int min_len  = dp[m-1][n-1];
    int last_pos = n-1;

    for (int i = m-1; i < n-1; ++i)
    {
        dp[m-1][i] += dist[i][n-1][0];

        if (dp[m-1][i] < min_len) 
        {
            min_len  = dp[m-1][i];
            last_pos = i;
        }
    }

    while(prev[m-1][last_pos] > 0)
    {
        positions[m-1] =coords[last_pos];
        last_pos = prev[m-1][last_pos];
        --m;
    }

    positions[0] = coords[last_pos];

    return min_len;
}

void FindDistancesBetweenPoints(std::vector<int> &coords, std::vector<std::vector<std::vector<int>>> &dist, int n)
{
    for (int i = 0; i < n-1; ++i) 
    {
        for (int j = i+1; j < n; ++j) 
        {
            dist[i][j][0] = dist[i][j-1][0] + coords[j] - coords[i];
        }
    }
    for (int i = n-1; i > 0; --i) 
    {
        for (int j = i-1; j >= 0; --j) 
        {
            dist[i][j][1] = dist[i][j+1][1] + coords[i] - coords[j];
        }
    }
}

int BinarySearch(std::vector<int> &coords, int pivot)
{
    auto mid_ptr = std::lower_bound(coords.begin(), coords.end(), pivot);
    int  mid_pos = static_cast<int>(std::distance(coords.begin(), mid_ptr)); 
    return mid_pos;
}