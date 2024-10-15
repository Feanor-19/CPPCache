#include <gtest/gtest.h>

#include "ideal_cache.hpp"

struct page_t
{
    int id;
    void *data;
};

page_t slow_get_page(int id)
{
    return {id, NULL};
}


TEST(IC, LookupUpdateTrivial) 
{
	std::vector<int> queries = {1, 1};

	IdealCache<page_t> IC(1, queries);

	bool first_lookup = IC.lookup_update(1, slow_get_page);
	EXPECT_FALSE(first_lookup);

	bool second_lookup = IC.lookup_update(1, slow_get_page);
	EXPECT_TRUE(second_lookup);
}

TEST(IC, LookupUpdateZeroCacheSize)
{
	std::vector<int> queries = {1};

	IdealCache<page_t> IC(0, queries);

	bool lookup = IC.lookup_update(1, slow_get_page);
	EXPECT_FALSE(lookup);
}

TEST(IC, EmptyQueriesCtor)
{
	std::vector<int> empty_queries;

	IdealCache<page_t> IC(19, empty_queries);
	// must not crash
}