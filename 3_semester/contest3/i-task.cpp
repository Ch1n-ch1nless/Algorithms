#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

std::vector<bool> used;
int timer;
std::vector<int> tin, ret, components;
std::vector<std::pair<int, int>> bridges;
std::vector<std::vector<int>> graph, newGraph;

void SplitGraphToComponents(int v, int component_id) 
{
    components[v] = component_id;
    used[v] = true;

    for (int to : newGraph[v]) 
    {
        if (!used[to]) 
        {
            SplitGraphToComponents(to, component_id);
        }
    }
}

void FindBrige(int v, int p) 
{
    tin[v] = timer++;
    ret[v] = tin[v];
    used[v] = true;

    for (int to : graph[v]) 
    {
        if (to == p) 
        {
            continue;
        }

        if (used[to]) 
        {
            ret[v] = std::min(ret[v], tin[to]);
        } else 
        {
            FindBrige(to, v);
            ret[v] = std::min(ret[v], ret[to]);

            if (ret[to] >= tin[to]) 
            {
                bridges.push_back({v, to});
            } else 
            {
                newGraph[v].push_back(to);
                newGraph[to].push_back(v);
            }
        }
    }
}

int main() 
{
    int n, m;
    std::cin >> n >> m;
    
    graph.resize(n);
    newGraph.resize(n);
    
    int v1 = 0;
    int v2 = 0;

    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2;
        v1--;
        v2--;

        graph[v1].push_back(v2);
        graph[v2].push_back(v1);
    }

    used.resize(n, false);
    timer = 0;
    tin.resize(n);
    ret.resize(n);
    bridges.clear();

    for (int i = 0; i < n; ++i) 
    {
        if (!used[i]) 
        {
            FindBrige(i, -1);
        }
    }

    used.assign(n, false);
    components.resize(n);

    int components_num = 0;

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            SplitGraphToComponents(i, components_num);
            ++components_num;
        }
    }

    std::vector<std::vector<int>> miniGraph(components_num);
    
    for (const auto& bridge : bridges) 
    {
        miniGraph[components[bridge.first]].push_back(components[bridge.second]);
        miniGraph[components[bridge.second]].push_back(components[bridge.first]);
    }

    int res = 0;

    for (int i = 0; i < components_num; ++i) 
    {
        if (miniGraph[i].size() == 1) 
        {
            ++res;
        }
    }

    std::cout << (res + 1) / 2 << '\n';

    return 0;
}
