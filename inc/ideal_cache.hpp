#pragma once

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cstdlib>

template <typename T, typename KeyT = int>
class IdealCache
{
    size_t cache_size_;

    std::vector<T> cache_;
    using CacheIt = typename std::vector<T>::iterator;

    std::unordered_map<KeyT, CacheIt> hash_;

    const std::vector<KeyT> &queries_; 
    using QueriesIt = typename std::vector<KeyT>::const_iterator;
    QueriesIt cur_plus_one_queries_it_;

public:
    explicit IdealCache(size_t cache_size, const std::vector<KeyT> &queries);

    template <typename F>
    bool lookup_update(KeyT id, F slow_get_page);
};

#include <algorithm>

template <typename T, typename KeyT>
IdealCache<T, KeyT>::IdealCache(size_t cache_size, const std::vector<KeyT> &queries):
    cache_size_(cache_size), queries_(queries)
{
    cache_.reserve(cache_size);
    cur_plus_one_queries_it_ = queries_.cbegin();
}

template <typename T, typename KeyT>
template <typename F>
bool IdealCache<T, KeyT>::lookup_update(KeyT id, F slow_get_page)
{
    cur_plus_one_queries_it_++;

    if (hash_.find(id) != hash_.end())
        return true;

    // if cache is not full yet
    size_t cur_size = 0;
    if ( (cur_size = hash_.size()) != cache_size_)
    {
        cache_[cur_size] = slow_get_page(id);
        hash_[id] = cache_.begin() + cur_size;
        return false;
    }

    std::unordered_set<KeyT> unmet_ids;
    for (auto it = hash_.cbegin(); it != hash_.cend(); ++it) 
        unmet_ids.insert(it->first);

    auto id_to_pop_it = cur_plus_one_queries_it_;
    for (; id_to_pop_it != queries_.cend() && unmet_ids.size() > 1; id_to_pop_it++)
        unmet_ids.erase(*id_to_pop_it);

    const KeyT id_to_pop = *(unmet_ids.cbegin());

    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = slow_get_page(id);

    hash_.erase(id_to_pop);
    hash_[id] = cache_it;

    return false;
}