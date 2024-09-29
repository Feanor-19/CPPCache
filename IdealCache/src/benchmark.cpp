#include <benchmark/benchmark.h>

#include <iostream>
#include <fstream>

#include "ideal_cache.hpp"

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

static void BM_IC_lookup_update(benchmark::State &state)
{
    //setup
    size_t cache_size = 0, queries_cnt = 0;

    std::ifstream file("../../../../E2E_tests/default.in");
    if (!file.is_open())
        std::cout << "Unable to open file" << std::endl; 

    file >> cache_size >> queries_cnt;

    std::vector<int> queries;
    queries.reserve(queries_cnt);

    int input = 0;
    while (queries_cnt-- && file >> input)
        queries.push_back(input);

    IdealCache<page_t> cache(cache_size, queries);
    for (auto _ : state)
    {
        //this code is timed
        size_t hits = 0;
        for (auto it = queries.cbegin(); it != queries.cend(); it++)
        {
            hits += cache.lookup_update(*it, slow_get_page);
        }
    }
}

BENCHMARK(BM_IC_lookup_update);

BENCHMARK_MAIN();