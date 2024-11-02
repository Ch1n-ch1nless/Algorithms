#include <iostream>
#include <vector>

class DisjointSetUnion
{
public:
     DisjointSetUnion(int set_number, std::vector<int> &data);
    ~DisjointSetUnion() = default;

    int  getParent(int set_index);
    int  uniteSets(int set1, int set2);

private:
    std::vector<int>    parent_;
    std::vector<int>    size_;
    std::vector<int>&   data_;
};

DisjointSetUnion::DisjointSetUnion(int set_number, std::vector<int> &data)
:
    parent_ (set_number),
    size_   (set_number, 1),
    data_   (data)
{
    for (int i = 0; i < set_number; i++)
    {
        parent_[i] = i;
    }
}

int DisjointSetUnion::getParent(int set_index)
{
    return (set_index == parent_[set_index]) ? 
            set_index                        : 
            parent_[set_index] = getParent(parent_[set_index]);
}

int DisjointSetUnion::uniteSets(int set1, int set2)
{
    set1 = getParent(set1);
    set2 = getParent(set2);

    if (set1 == set2) return data_[set1];

    if (size_[set1] < size_[set2])
    {
        std::swap(set1, set2);
    }

    size_[set1] += size_[set2];
    data_[set1] += data_[set2];
    parent_[set2] = set1;

    return data_[set1];
}

int main()
{
    int tables_number   = 0;
    int request_number  = 0;

    std::cin >> tables_number >> request_number;

    std::vector<int> table(tables_number);
    int max_size = 0;

    for (int i = 0; i < tables_number; i++)
    {
        std::cin >> table[i];
        max_size = std::max(max_size, table[i]);
    }

    DisjointSetUnion data_base(tables_number, table);

    for (int i = 0; i < request_number; i++)
    {
        int dest = 0;
        int src  = 0;

        std::cin >> dest >> src;

        dest--;
        src--;

        max_size = std::max(max_size, data_base.uniteSets(dest, src));

        std::cout << max_size << '\n';
    }

    return 0;
}