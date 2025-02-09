#ifndef STATIC_VECTOR_HPP_
#define STATIC_VECTOR_HPP_

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <type_traits>

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)

// -------------------------------------------------------------------------------------------------
template <typename Element, size_t CAPACITY>
struct UninitializedArray {
  static constexpr bool is_trivial_enough = std::is_trivially_default_constructible_v<Element> &&
                                            std::is_trivially_destructible_v<Element>;

  using Storage_t = std::conditional_t<is_trivial_enough,
                                       Element[CAPACITY],                       // NOLINT
                                       std::byte[CAPACITY * sizeof(Element)]>;  // NOLINT
  alignas(Element) Storage_t m_data;

  constexpr auto data() noexcept -> Element* {
    if constexpr (is_trivial_enough) {
      return m_data;
    } else {
      return reinterpret_cast<Element*>(m_data);
    }
  }
  constexpr auto data() const noexcept -> const Element* {
    if constexpr (is_trivial_enough) {
      return m_data;
    } else {
      return reinterpret_cast<const Element*>(m_data);
    }
  }
};

// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

// -------------------------------------------------------------------------------------------------
template <typename Element, size_t CAPACITY>
class StaticVector {
  UninitializedArray<Element, CAPACITY> m_data;
  size_t m_size = 0UZ;

 public:
  static constexpr auto is_trivial_enough =
      UninitializedArray<Element, CAPACITY>::is_trivial_enough;

  constexpr StaticVector() noexcept = default;
  constexpr StaticVector(std::initializer_list<Element> values) noexcept {
    // assert(values.size() <= CAPACITY && "Size is greater than capacity.");
    // m_size = values.size();
    // for (size_t i = 0; i < values.size(); ++i) {
    //   std::construct_at(reinterpret_cast<Element*>(m_data + offset(i)),
    //                     std::move(*std::next(values.begin(), static_cast<long>(i))));
    // }
    for (auto& v : values) {
      push_back(v);
    }
  }

  constexpr StaticVector(const StaticVector& other) noexcept = delete;
  constexpr StaticVector(StaticVector&& other) noexcept      = delete;

  constexpr auto operator=(const StaticVector& other) noexcept -> StaticVector& = delete;
  constexpr auto operator=(StaticVector&& other) noexcept -> StaticVector&      = delete;

  constexpr ~StaticVector() noexcept {
    for (size_t i = 0; i < m_size; ++i) {
      std::destroy_at(m_data.data() + i);
    }
  }

  constexpr void push_back(const Element& e) noexcept {
    assert(m_size < CAPACITY && "Size may not exceed capacity.");
    std::construct_at(m_data.data() + m_size, e);
    m_size += 1;
  }
  constexpr void push_back(Element&& e) noexcept {
    assert(m_size < CAPACITY && "Size may not exceed capacity.");
    std::construct_at(m_data.data() + m_size, std::move(e));
    m_size += 1;
  }

  [[nodiscard]] constexpr auto operator[](size_t idx) noexcept -> Element& {
    return *(m_data.data() + idx);
  }
  [[nodiscard]] constexpr auto operator[](size_t idx) const noexcept -> const Element& {
    return *(m_data.data() + idx);
  }

  [[nodiscard]] constexpr auto size() const noexcept -> size_t { return m_size; }
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_t { return CAPACITY; }
};

#endif  // STATIC_VECTOR_HPP_
