#include <gtest/gtest.h>

#include <memory>

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

// -------------------------------------------------------------------------------------------------
TEST(Initialize, CopyConstructor) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with same capacity
  {
    const StaticVector<std::shared_ptr<int>, 8UZ> v2 = v1;  // NOLINT
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with more capacity
  {
    const StaticVector<std::shared_ptr<int>, 16UZ> v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with less capacity
  {
    const StaticVector<std::shared_ptr<int>, 4UZ> v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(Initialize, MoveConstructor) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);

  // Move to vector with same capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const StaticVector<std::shared_ptr<int>, 8UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Copy to vector with more capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const StaticVector<std::shared_ptr<int>, 16UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with less capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    const StaticVector<std::shared_ptr<int>, 4UZ> v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(Initialize, CopyAssignment) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);
  const auto p5 = std::make_shared<int>(5);

  StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
  EXPECT_EQ(p5.use_count(), 1UZ);

  // Copy to vector with same capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with more capacity
  {
    StaticVector<std::shared_ptr<int>, 16UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);

  // Copy to vector with less capacity
  {
    StaticVector<std::shared_ptr<int>, 4UZ> v2{p5};
    v2 = v1;
    EXPECT_EQ(p1.use_count(), 3UZ);
    EXPECT_EQ(p2.use_count(), 3UZ);
    EXPECT_EQ(p3.use_count(), 3UZ);
    EXPECT_EQ(p4.use_count(), 3UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 2UZ);
  EXPECT_EQ(p2.use_count(), 2UZ);
  EXPECT_EQ(p3.use_count(), 2UZ);
  EXPECT_EQ(p4.use_count(), 2UZ);
}

// -------------------------------------------------------------------------------------------------
TEST(Initialize, MoveAssignment) {
  const auto p1 = std::make_shared<int>(1);
  const auto p2 = std::make_shared<int>(2);
  const auto p3 = std::make_shared<int>(3);
  const auto p4 = std::make_shared<int>(4);
  const auto p5 = std::make_shared<int>(5);

  // Move to vector with same capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    StaticVector<std::shared_ptr<int>, 8UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with more capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    StaticVector<std::shared_ptr<int>, 16UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);

  // Move to vector with less capacity
  {
    StaticVector<std::shared_ptr<int>, 8UZ> v1{p1, p2, p3, p4};
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    StaticVector<std::shared_ptr<int>, 4UZ> v2{p5};
    v2 = std::move(v1);
    EXPECT_EQ(p1.use_count(), 2UZ);
    EXPECT_EQ(p2.use_count(), 2UZ);
    EXPECT_EQ(p3.use_count(), 2UZ);
    EXPECT_EQ(p4.use_count(), 2UZ);
    EXPECT_EQ(p5.use_count(), 1UZ);

    for (size_t i = 0; i < v2.size(); ++i) {
      EXPECT_EQ(*v2[i], static_cast<int>(i + 1));
    }
  }
  EXPECT_EQ(p1.use_count(), 1UZ);
  EXPECT_EQ(p2.use_count(), 1UZ);
  EXPECT_EQ(p3.use_count(), 1UZ);
  EXPECT_EQ(p4.use_count(), 1UZ);
}
