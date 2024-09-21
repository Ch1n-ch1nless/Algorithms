#include <cstdio>
#include <vector>

unsigned int FindLenOfLargestCommonSequence(std::vector<unsigned int>& seq1, std::vector<unsigned int>& seq2)
{
    //Create array
    unsigned int** dp = new unsigned int*[seq1.size() + 1];

    for (unsigned int i = 0; i <= seq1.size(); ++i)
    {
        dp[i] = new unsigned int[seq2.size()];
    }

    //Clear array
    for (unsigned int i = 0; i <= seq1.size(); ++i)
    {
        for (unsigned int j = 0; j < seq2.size(); ++j)
        {
            dp[i][j] = 0;
        }
    }

    for (unsigned int i = 1; i <= seq1.size(); ++i)
    {
        unsigned int max_len = 0;

        for (unsigned int j = 0; j < seq2.size(); ++j)
        {
            dp[i][j] = dp[i-1][j];

            if (seq1[i-1] == seq2[j] && dp[i-1][j] <=  max_len)
            {
                dp[i][j] = max_len + 1;
            }
            else if (seq1[i-1] > seq2[j] && dp[i-1][j] > max_len)
            {
                max_len = dp[i-1][j];
            }
        }
    }

    unsigned int max_len = 0;

    for (unsigned int i = 0; i < seq2.size(); ++i)
    {
        max_len = (max_len > dp[seq1.size()][i]) ? max_len : dp[seq1.size()][i];
    }    

    //Destroy array
    for (unsigned int i = 0; i <= seq1.size(); ++i)
    {
        delete[] dp[i];
    }

    delete dp;

    return max_len;
}

void ReadDance(std::vector<unsigned int>& dance, unsigned int dance_len)
{
    unsigned int dance_elem = 0;

    for (unsigned int i = 0; i < dance_len; ++i)
    {
        scanf("%u", &dance_elem);
        dance.push_back(dance_elem);
    }
}

int main()
{
    unsigned int cj_dance_len = 0;
    unsigned int dj_dance_len = 0;

    scanf("%u %u", &cj_dance_len, &dj_dance_len);

    std::vector<unsigned int> cj_dance = {};
    std::vector<unsigned int> dj_dance = {};

    ReadDance(cj_dance, cj_dance_len);
    ReadDance(dj_dance, dj_dance_len);

    unsigned int answer = FindLenOfLargestCommonSequence(cj_dance, dj_dance);

    printf("%u\n", answer);

    return 0;
}