#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <string>

using namespace std::string_literals;

#include "StaticVector.hpp"

static_assert(std::contiguous_iterator<decltype(std::declval<StaticVector<int, 16>>().begin())>,
              "ForwardIterator must be contiguous iterator.");
static_assert(std::contiguous_iterator<decltype(std::declval<StaticVector<int, 16>>().cbegin())>,
              "Const ForwardIterator must be contiguous iterator.");
// TODO:
// static_assert(std::random_access_iterator<decltype(std::declval<StaticVector<int,
// 16>>().rbegin())>,
//               "ReverseIterator must be contiguous iterator.");
// static_assert(
//     std::random_access_iterator<decltype(std::declval<StaticVector<int, 16>>().crbegin())>,
//     "Const ReverseIterator must be contiguous iterator.");

TEST(Iterator, ForwardIterator) {
  {
    StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    EXPECT_FALSE(std::is_sorted(std::cbegin(vec), std::cend(vec)));

    std::sort(std::begin(vec), std::end(vec));
    EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
  }

  {
    StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    EXPECT_FALSE(std::is_sorted(std::cbegin(vec), std::cend(vec)));

    std::sort(std::begin(vec), std::end(vec));
    EXPECT_TRUE(std::is_sorted(std::cbegin(vec), std::cend(vec)));
  }
}

TEST(Iterator, ReverseIterator) {
  {
    StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    EXPECT_FALSE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_FALSE(std::is_sorted(std::rbegin(vec), std::rend(vec)));

    std::sort(std::rbegin(vec), std::rend(vec));
    EXPECT_TRUE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_TRUE(std::is_sorted(std::rbegin(vec), std::rend(vec)));
  }

  {
    StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    EXPECT_FALSE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_FALSE(std::is_sorted(std::rbegin(vec), std::rend(vec)));

    std::sort(std::rbegin(vec), std::rend(vec));
    EXPECT_TRUE(std::is_sorted(std::crbegin(vec), std::crend(vec)));
    EXPECT_TRUE(std::is_sorted(std::rbegin(vec), std::rend(vec)));
  }
}

TEST(Iterator, ModifyReverseIterator) {
  {
    StaticVector<int, 16UZ> vec{3, 4, 5, 1, 2, 3, 9, 8, 5, 1001};
    const auto rbegin = vec.rbegin();
    *rbegin           = 2002;
    EXPECT_EQ(vec.back(), 2002);

    const auto rnext = std::next(rbegin, 2);
    *rnext           = -8;
    EXPECT_EQ(vec[7], -8);

    const auto rprev = std::prev(std::next(rnext, 2), 1);
    *rprev           = -9;
    EXPECT_EQ(vec[6], -9);
  }

  {
    StaticVector<std::string, 16UZ> vec{
        "3"s, "4"s, "5"s, "1"s, "2"s, "3"s, "9"s, "8"s, "5"s, "1001"s};
    const auto rbegin = vec.rbegin();
    *rbegin           = "2002"s;
    EXPECT_EQ(vec.back(), "2002"s);

    const auto rnext = std::next(rbegin, 2);
    *rnext           = "-8"s;
    EXPECT_EQ(vec[7], "-8"s);

    const auto rprev = std::prev(std::next(rnext, 2), 1);
    *rprev           = "-9"s;
    EXPECT_EQ(vec[6], "-9"s);
  }
}
