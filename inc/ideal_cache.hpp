#pragma once

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cstdlib>

template <typename T, typename KeyT = int>
class IdealCache
{
    size_t size_;

    std::vector<T> cache_;
    using CacheIt = std::vector<T>::iterator;

    std::unordered_map<KeyT, CacheIt> hash_;

    const std::vector<KeyT> &queries_; 
    using QueriesIt = std::vector<KeyT>::const_iteraror;
    QueriesIt cur_queries_it;

public:
    IdealCache(size_t cache_size, const std::vector<KeyT> &queries);

    template <typename F>
    bool lookup_update(KeyT id, F slow_get_page);
};
