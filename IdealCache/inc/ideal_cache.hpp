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

    const std::vector<KeyT> queries_; 
    using QueriesIt = typename std::vector<KeyT>::const_iterator;
    QueriesIt cur_plus_one_queries_it_;

    using QueriesItVec = typename std::vector<QueriesIt>;
    using QIVIt = typename QueriesItVec::const_iterator;

    std::unordered_map<KeyT, std::pair<QIVIt, QueriesItVec>> queries_by_id_;

    bool will_meet_id(KeyT id) const;
    KeyT choose_id_to_pop() const;

public:
    IdealCache(size_t cache_size, const std::vector<KeyT> &queries);

    template <typename F>
    bool lookup_update(KeyT id, F slow_get_page);

    bool full() const;
};


template <typename T, typename KeyT>
IdealCache<T, KeyT>::IdealCache(size_t cache_size, const std::vector<KeyT> &queries) 
    : cache_size_(cache_size), queries_(queries)
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
inline bool IdealCache<T, KeyT>::will_meet_id(KeyT id) const
{
    const auto &[qiv_it, queries_it_vec] = queries_by_id_.at(id);

    return qiv_it != queries_it_vec.cend();
}

template <typename T, typename KeyT>
inline KeyT IdealCache<T, KeyT>::choose_id_to_pop() const
{
    KeyT id_to_pop = (hash_.begin())->first;
    typename std::vector<KeyT>::difference_type max_diff = 0;
    for (auto &elem : hash_)
    {
        KeyT id = elem.first;
        const QIVIt &qiv_it = queries_by_id_.at(id).first;
        const QueriesItVec &queries_it_vec = queries_by_id_.at(id).second;

        if (!will_meet_id(id))
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
    return id_to_pop;
}

template <typename T, typename KeyT>
inline bool IdealCache<T, KeyT>::full() const
{
    return hash_.size() == cache_size_;
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

    if (!will_meet_id(id)) 
        return false;

    if (!full())
    {
        cache_.push_back(slow_get_page(id));
        hash_[id] = cache_.end() - 1;
        return false;
    }

    KeyT id_to_pop = choose_id_to_pop();

    CacheIt cache_it = hash_[id_to_pop];
    *cache_it = slow_get_page(id);

    hash_.erase(id_to_pop);
    hash_[id] = cache_it;

    return false;
}