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

    using QueriesItVec = typename std::vector<QueriesIt>;
    using QIVIt = typename QueriesItVec::const_iterator;

    std::unordered_map<KeyT, std::pair<QIVIt, QueriesItVec>> queries_by_id_;

public:
    IdealCache(size_t cache_size, const std::vector<KeyT> &queries);

    template <typename F>
    bool lookup_update(KeyT id, F slow_get_page);
};

template <typename T, typename KeyT>
IdealCache<T, KeyT>::IdealCache(size_t cache_size, const std::vector<KeyT> &queries):
    cache_size_(cache_size), queries_(queries)
{
    cache_.reserve(cache_size);
    cur_plus_one_queries_it_ = queries_.cbegin();

    for (QueriesIt it = queries_.cbegin(); it != queries_.cend(); it++)
    {
        QueriesItVec &queries_it_vec = queries_by_id_[*it].second;
        queries_it_vec.push_back(it);
    }
        
    for (auto &elem : queries_by_id_)
    {
        QIVIt &qiv_it = elem.second.first;
        qiv_it = elem.second.second.begin();
    }
}

template <typename T, typename KeyT>
template <typename F>
bool IdealCache<T, KeyT>::lookup_update(KeyT id, F slow_get_page)
{
    if (cache_size_ == 0)
    {
        slow_get_page(id);
        return false;
    }

    cur_plus_one_queries_it_++;
    queries_by_id_[id].first++;

    if (hash_.find(id) != hash_.end())
        return true;

    // if the id we're searching place for won't be met anymore, we don't need to save it in cache
    if (queries_by_id_[id].first == queries_by_id_[id].second.cend()) 
        return false;

    // if cache is not full yet
    if (hash_.size() != cache_size_)
    {
        cache_.push_back(slow_get_page(id));
        hash_[id] = cache_.end() - 1;
        return false;
    }

    // choose which element to pop from cache
    KeyT id_to_pop = (hash_.begin())->first;
    typename std::vector<KeyT>::difference_type max_diff = 0;
    for (auto &elem : hash_)
    {
        KeyT id = elem.first;
        const QIVIt &qiv_it = queries_by_id_[id].first;
        const QueriesItVec &queries_it_vec = queries_by_id_[id].second;

        if (qiv_it == queries_it_vec.cend()) // if this id won't be met anymore
        {
            id_to_pop = id;
            break;
        }

        if (*qiv_it - cur_plus_one_queries_it_ > max_diff)
        {
            id_to_pop = id;
            max_diff = *qiv_it - cur_plus_one_queries_it_;
        }
    }

    // remove old one and insert new one
    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = slow_get_page(id);

    hash_.erase(id_to_pop);
    hash_[id] = cache_it;

    return false;
}