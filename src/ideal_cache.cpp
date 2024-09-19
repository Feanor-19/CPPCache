#include "ideal_cache.hpp"

template <typename T, typename KeyT>
IdealCache<T, KeyT>::IdealCache(size_t cache_size, const std::vector<KeyT> &queries):
    cache_(cache_size), queries_(queries)
{
    cache_.reserve(cache_size);
    cur_queri_it = cache_.cbegin();
}

template <typename T, typename KeyT>
template <typename F>
bool IdealCache<T, KeyT>::lookup_update(KeyT id, F slow_get_page)
{
    if ( hash_.find(id) != hash_.end())
    {
        cur_queries_it++;
        return true;
    }

    std::unordered_set<KeyT> met_ids;
    size_t hash_size = hash_.size();
    auto it = cur_queries_it;
    KeyT last_met_id = *(hash_.begin()); // so it is a real id anyway
    for (; it != queries_.end(); ++it)
    {
        if (met_ids.size() == hash_size - 1)
            break;
        
        met_ids.insert(*it);
        last_met_id = 
    }

    if (it == queries_.end())
    KeyT id_to_pop = *it;

    // get index in cache from map, delete from map and cache, add new with F
    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = 

    cur_queries_it++;
    return false;
}