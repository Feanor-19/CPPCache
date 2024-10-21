#include "utilities.hpp"

#include <iostream>
#include <exception>

void CacheUtilities::get_input(int32_t &cache_size_out, std::vector<int> &queries_out)
{
    int32_t cache_size = 0, queries_cnt = 0;
    std::cin >> cache_size >> queries_cnt;

    if (!std::cin.good() || cache_size < 0 || queries_cnt < 0)
    {
        throw std::runtime_error("Wrong input");
    }

    std::vector<int> queries;
    queries.reserve(queries_cnt);

    int input = 0;
    while (queries_cnt-- && std::cin >> input)
        queries.push_back(input);

    if (queries_cnt > 0 || !std::cin)
    {
        throw std::runtime_error("Not full input");
    }

    cache_size_out = cache_size;
    queries_out    = queries; 
}