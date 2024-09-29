#pragma once

#include <unordered_map>
#include <map>
#include <vector>
#include <unordered_set>
#include <cstdlib>

template <typename T, typename KeyT = int>
class LFUCache
{
    size_t cache_size_;

    std::vector<T> cache_;
    using CacheIt = typename std::vector<T>::iterator;

    std::unordered_map<KeyT, CacheIt> hash_;

    std::map<KeyT, size_t> freq_;

public:
    explicit LFUCache(size_t cache_size);

    template <typename F>
    bool lookup_update(KeyT id, F slow_get_page);
};

template <typename T, typename KeyT>
LFUCache<T, KeyT>::LFUCache(size_t cache_size):
    cache_size_(cache_size)
{
    cache_.reserve(cache_size);
}

template <typename T, typename KeyT>
template <typename F>
bool LFUCache<T, KeyT>::lookup_update(KeyT id, F slow_get_page)
{
    if (cache_size_ == 0)
    {
        slow_get_page(id);
        return false;
    }

    if (hash_.find(id) != hash_.end())
    {
        freq_[id]++;
        return true;
    }

    // if cache is not full yet
    if (hash_.size() != cache_size_)
    {
        cache_.push_back(slow_get_page(id));
        hash_[id] = cache_.end() - 1;
        freq_[id] = 1;
        return false;
    }

    // choose which element to pop from cache
    size_t min_val = (size_t) -1;
    KeyT id_to_pop = (freq_.begin())->first;
    for (const std::pair<KeyT, size_t>& elem : freq_)
    {
        KeyT id = elem.first;
        size_t id_freq = elem.second;

        if (id_freq < min_val)
        {
            min_val = id_freq;
            id_to_pop = id;
        }
    }

    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = slow_get_page(id);
    
    hash_.erase(id_to_pop);
    hash_[id] = cache_it;

    freq_.erase(id_to_pop);
    freq_[id] = 1;

    return false;
}