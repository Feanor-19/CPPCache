#include <gtest/gtest.h>

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

TEST(LFUC, LookupUpdateTrivial) 
{
	LFUCache<page_t> LFUC(1);

	bool first_lookup = LFUC.lookup_update(1, slow_get_page);
	EXPECT_FALSE(first_lookup);

	bool second_lookup = LFUC.lookup_update(1, slow_get_page);
	EXPECT_TRUE(second_lookup);
}

TEST(LFUC, LookupUpdateZeroCacheSize)
{
	LFUCache<page_t> LFUC(0);

	bool lookup = LFUC.lookup_update(1, slow_get_page);
	EXPECT_FALSE(lookup);
}

