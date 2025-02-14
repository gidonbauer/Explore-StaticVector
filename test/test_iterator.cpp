#include <gtest/gtest.h>

#include <algorithm>

#include "StaticVector.hpp"

TEST(Iterator, ForwardIterator) {
  StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
  EXPECT_FALSE(std::is_sorted(std::cbegin(vec), std::cend(vec)));

  std::sort(std::begin(vec), std::end(vec));
  EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
}

TEST(Iterator, ReverseIterator) {
  StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
  // EXPECT_FALSE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
  EXPECT_FALSE(std::is_sorted(std::rbegin(vec), std::rend(vec)));

  std::sort(std::rbegin(vec), std::rend(vec));
  // EXPECT_TRUE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
  EXPECT_TRUE(std::is_sorted(std::rbegin(vec), std::rend(vec)));
}
