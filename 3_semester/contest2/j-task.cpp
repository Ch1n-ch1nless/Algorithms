#include <iostream>
#include <limits>
#include <vector>

static const long long INF              =  1ll << 41 + 1;
static const long long COEFFICIENT_K    = -2ll;
static const long long COEFFICIENT_B    =  1ll; 

class Line
{
public:
    Line() {}
    Line(const long long& k, const long long& b) { k_ = k; b_ = b;}

    long long GetY(const long long& x)
    {
        return k_ * x + b_;
    }

    long long GetK() { return k_; }
    long long GetB() { return b_; }

private:
    long long k_;
    long long b_;
};

long long CrossLines(Line& line1, Line& line2)
{
    long long x = (line2.GetB() - line1.GetB()) / (line1.GetK() - line2.GetK());
    
    if (line2.GetB() < line1.GetB()) x--;

    return x;
}

class ConvexHullTrick
{
public:
     ConvexHullTrick() = default;
    ~ConvexHullTrick() = default;

    void        AddLine (Line new_line);
    long long   GetMin  (const long long& x);

private:
    std::vector<long long>  points_;
    std::vector<Line>       lines_;
};

void ConvexHullTrick::AddLine(Line new_line)
{
    while (lines_.size() > 0 && (lines_.back().GetY(points_.back()) > new_line.GetY(points_.back())))
    {
        lines_.pop_back();
        points_.pop_back();
    }

    if (lines_.empty()) 
    {
        points_.push_back(-INF);
    }
    else
    {
        
        points_.push_back(CrossLines(lines_.back(), new_line));
    }
    lines_.push_back(new_line);
}

long long ConvexHullTrick::GetMin(const long long& x)
{
    long long line_index = std::lower_bound(points_.begin(), points_.end(), x) - points_.begin() - 1;
    
    return lines_[line_index].GetY(x);
}

long long FindMinSumSquareLens(const long long& N, const long long& C, const std::vector<long long>& height)
{
    std::vector<long long> dp(N, INF);

    dp[0] = 0ll;

    ConvexHullTrick trick;

    for (long long i = 1; i < N; i++)
    {
        trick.AddLine(Line(-2ll * height[i-1], height[i-1] * height[i-1] + dp[i-1] + C));
        dp[i] = trick.GetMin(height[i]) + height[i] * height[i];
    }

    if (dp[N-1] < INF)
    {
        return dp[N-1];
    }

    return 0;
}

int main()
{
    long long N = 0;
    long long C = 0;

    std::cin >> N >> C;

    std::vector<long long> heights(N);

    for (long long i = 0; i < N; ++i)
    {
        std::cin >> heights[i];
    }

    long long answer = FindMinSumSquareLens(N, C, heights);

    std::cout << answer << '\n';

    return 0;
}