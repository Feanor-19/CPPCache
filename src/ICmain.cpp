#include "ideal_cache.hpp"

#include <iostream>

struct page_t
{
    int id;
    void *data;
};

page_t slow_get_page(int id)
{
    // it should be slow on its own?
    return {id, NULL};
}

int main(int, char**)
{
    size_t cache_size = 0, elems_cnt = 0;
    std::cin >> cache_size >> elems_cnt;

    std::vector<int> queries;
    queries.reserve(elems_cnt);

    int page_id = 0;
    for (size_t i = 0; i != elems_cnt; i++)
        std::cin >> queries[i];
    
    IdealCache<page_t> cache(cache_size, queries);
    
    size_t hits = 0;
    for (auto it = queries.cbegin(); it != queries.cend(); it++)
        hits += cache.lookup_update(*it, slow_get_page);

    std::cout << hits;
}
