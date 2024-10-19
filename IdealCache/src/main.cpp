#include "ideal_cache.hpp"

#include <iostream>

struct page_t
{
    int id;
    void *data;
};

page_t slow_get_page(int id)
{
    return {id, NULL};
}

int main(int, char**)
{
    int32_t cache_size = 0, queries_cnt = 0;
    std::cin >> cache_size >> queries_cnt;

    if (!std::cin.good() || cache_size < 0 || queries_cnt < 0)
    {
        std::cerr << "ERROR: Wrong input!" << std::endl;
        return 1;
    }

    std::vector<int> queries;
    queries.reserve(queries_cnt);

    int input = 0;
    while (queries_cnt-- && std::cin >> input)
        queries.push_back(input);

    if (queries_cnt > 0 || !std::cin)
    {
        std::cerr << "ERROR: Not full input!" << std::endl;
        return 2;
    }

    IdealCache<page_t> cache(cache_size, queries);
    
    size_t hits = 0;
    for (auto it = queries.cbegin(); it != queries.cend(); it++)
    {
        hits += cache.lookup_update(*it, slow_get_page);
    }
    std::cout << hits << std::endl;
}
