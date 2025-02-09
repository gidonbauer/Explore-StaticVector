#include <gtest/gtest.h>

#include "StaticVector.hpp"

// -------------------------------------------------------------------------------------------------
TEST(Initialize, DefaultConstructor) {
  {
    StaticVector<int, 8UZ> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 8UZ);
  }

  {
    struct S {
      S()                            = delete;
      S(const S&)                    = delete;
      S(S&&)                         = delete;
      auto operator=(const S&) -> S& = delete;
      auto operator=(S&&) -> S&      = delete;
      ~S()                           = default;
    };

    StaticVector<S, 8UZ> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 8UZ);
  }
}

// -------------------------------------------------------------------------------------------------
TEST(Initialize, SizeInitializer) {
  {
    StaticVector<int, 16UZ> vec(10UZ);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, int{});
    }
  }

  {
    StaticVector<int, 16UZ> vec(10UZ, 42);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, 42);
    }
  }

  {
    StaticVector<std::string, 16UZ> vec(10UZ);
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e, std::string{});
    }
  }

  {
    StaticVector<std::string, 16UZ> vec(
        10UZ,
        std::string{"A very long string that should not fit into the small string optimization."});
    EXPECT_EQ(vec.size(), 10UZ);
    for (const auto& e : vec) {
      EXPECT_EQ(e,
                std::string{
                    "A very long string that should not fit into the small string optimization."});
    }
  }

  {
    using SV = StaticVector<int, 8UZ>;
    EXPECT_DEATH(SV vec(10UZ);, "");
  }
}

// -------------------------------------------------------------------------------------------------
TEST(Initialize, InitializerList) {
  {
    StaticVector<int, 16> vec{1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_EQ(vec.size(), 8);
    for (size_t i = 0; i < vec.size(); ++i) {
      EXPECT_EQ(i + 1, vec[i]);
    }
  }

  {
    StaticVector<std::array<float, 2>, 16> vec{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    ASSERT_EQ(vec.size(), 4);
    for (size_t i = 0; i < vec.size(); ++i) {
      EXPECT_FLOAT_EQ(static_cast<float>(2 * i + 1), vec[i][0]);
      EXPECT_FLOAT_EQ(static_cast<float>(2 * i + 2), vec[i][1]);
    }
  }
}
