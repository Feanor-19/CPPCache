#include "lfu_cache.hpp"
#include "utilities.hpp"

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
    int32_t cache_size = 0;
    std::vector<int> queries{};
    try
    {
        CacheUtilities::get_input(cache_size, queries);
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << "!" << '\n';
        return 1;
    }

    LFUCache<page_t> cache(cache_size);
    
    size_t hits = 0;
    for (auto &query : queries)
    {
        hits += cache.lookup_update(query, slow_get_page);
    }
    std::cout << hits << std::endl;
}
