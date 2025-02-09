#include <gtest/gtest.h>

#include <fmt/format.h>

#include "StaticVector.hpp"

TEST(TestSV, VectorOfInt) {
  {
    static_assert(StaticVector<int, 8UZ>::is_trivial_enough);
    StaticVector<int, 8UZ> sv;

    sv.push_back(4);
    sv.push_back(2);

    for (size_t i = 0; i < sv.size(); ++i) {
      fmt::println("sv[{}] = {}", i, sv[i]);
    }
  }

  {
    constexpr StaticVector<int, 8UZ> sv({1, 2, 3, 4, 5, 6, 7, 8});
    fmt::println("sv.size() = {}", sv.size());

    fmt::print("sv = {{ ");
    for (size_t i = 0; i < sv.size(); ++i) {
      fmt::print("[{}] = {}, ", i, sv[i]);
    }
    fmt::println("}}");
  }
}

TEST(TestSV, VectorOfCustom) {
  struct S {
    int i;

    S() = delete;
    S(int i) noexcept
        : i(i) {
      std::puts("S(int)");
    }
    S(const S& other) noexcept
        : i(other.i) {
      std::puts("S(const S&)");
    }
    S(S&& other) noexcept
        : i(other.i) {
      std::puts("S(S&&)");
    }
    auto operator=(const S& other) noexcept -> S& {
      if (this != &other) { i = other.i; }
      std::puts("operator=(const S&)");
      return *this;
    }
    auto operator=(S&& other) noexcept -> S& {
      i = other.i;
      std::puts("operator=(S&&)");
      return *this;
    }
    ~S() noexcept { std::puts("~S()"); }
  };

  StaticVector<S, 8UZ> sv;

  S s(1);
  sv.push_back(s);
  sv.push_back(S(2));

  for (size_t i = 0; i < sv.size(); ++i) {
    fmt::println("sv[{}].i = {}", i, sv[i].i);
  }
}
