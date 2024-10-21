#include <benchmark/benchmark.h>

#include <iostream>
#include <fstream>

#include "lfu_cache.hpp"

struct page_t
{
    int id;
    void *data;
};

page_t slow_get_page(int id)
{
    return {id, NULL};
}

template <class ...Args>
static void BM_LFUC_lookup_update(benchmark::State &state, Args&&... args)
{
    //benchmark arg setup
    auto args_tuple = std::make_tuple(std::move(args)...);
    std::string bench_data_filename = std::get<0>(args_tuple); 

    //setup
    size_t cache_size = 0, queries_cnt = 0;

    std::ifstream file(bench_data_filename);
    if (!file.is_open())
    {
        state.SkipWithError("Can't open file");
        return;
    }

    file >> cache_size >> queries_cnt;

    std::vector<int> queries;
    queries.reserve(queries_cnt);

    int input = 0;
    while (queries_cnt-- && file >> input)
        queries.push_back(input);

    size_t hits = 0;
    for (auto _ : state)
    {
        //this code is timed
        LFUCache<page_t> cache(cache_size);
        hits = 0;
        for (auto it = queries.cbegin(); it != queries.cend(); it++)
        {
            hits += cache.lookup_update(*it, slow_get_page);
        }
    }
    std::cout << "Hits: " << hits << std::endl;
}

//BENCHMARK(BM_LFUC_lookup_update);

BENCHMARK_CAPTURE(BM_LFUC_lookup_update, 1000_100000_10000, \
                  std::string("benchmark_data/1000_100000_10000.dat"));

BENCHMARK_CAPTURE(BM_LFUC_lookup_update, problem_lc_015, \
                  std::string("benchmark_data/problem_lc_015.dat"));

BENCHMARK_MAIN();