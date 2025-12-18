#include <gtest/gtest.h>
#include "7/LruCache.hpp"

TEST(LruTest, CapacityLimit) {
    LruCache cache(2);
    cache.put("A", "1");
    cache.put("B", "2");
    cache.put("C", "3"); 
    EXPECT_FALSE(cache.get("A").has_value());
    EXPECT_TRUE(cache.get("B").has_value());
    EXPECT_TRUE(cache.get("C").has_value());
}

TEST(LruTest, RefreshOnGet) {
    LruCache cache(2);
    cache.put("A", "1");
    cache.put("B", "2");
    cache.get("A");       
    cache.put("C", "3");  
    EXPECT_TRUE(cache.get("A").has_value());
    EXPECT_FALSE(cache.get("B").has_value());
}