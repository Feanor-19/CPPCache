#include "utilities.hpp"

#include <iostream>
#include <exception>

std::vector<int> CacheUtilities::get_queries(int32_t &cache_size_ret)
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

    return queries;
}