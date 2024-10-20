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
    
    using freq_t = size_t; 
    std::multimap<freq_t, KeyT> freq_;
    using FreqIt = typename std::multimap<freq_t, KeyT>::iterator;

    std::unordered_map<KeyT, FreqIt> freq_it_by_id_;

public:
    LFUCache(size_t cache_size);

    bool full();

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
inline bool LFUCache<T, KeyT>::full()
{
    return hash_.size() == cache_size_;
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
        FreqIt old_freq_it = freq_it_by_id_[id];
        freq_t freq_val = old_freq_it->first;
        freq_.erase(old_freq_it);

        FreqIt new_freq_it = freq_.emplace(freq_val+1, id); 
        freq_it_by_id_[id] = new_freq_it;

        return true;
    }

    if (!full())
    {
        cache_.push_back(slow_get_page(id));
        hash_[id] = cache_.end() - 1;
        FreqIt freq_it = freq_.emplace(1, id);
        freq_it_by_id_[id] = freq_it;
        return false;
    }

    KeyT id_to_pop = (freq_.begin())->second;

    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = slow_get_page(id);
    
    hash_.erase(id_to_pop);
    hash_[id] = cache_it;

    FreqIt freq_it = freq_it_by_id_[id_to_pop];
    freq_it_by_id_.erase(id_to_pop);
    freq_.erase(freq_it);

    freq_it_by_id_[id] = freq_.emplace(1, id);

    return false;
}