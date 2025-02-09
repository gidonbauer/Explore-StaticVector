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
  using value_type      = Element;
  using size_type       = size_t;
  using difference_type = long int;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = value_type*;
  using const_pointer   = const value_type*;
  using iterator        = pointer;
  using const_iterator  = const_pointer;
  // using reverse_iterator       = void;
  // using const_reverse_iterator = void;

  static constexpr auto is_trivial_enough =
      UninitializedArray<Element, CAPACITY>::is_trivial_enough;

  constexpr StaticVector() noexcept = default;
  constexpr StaticVector(size_t size, Element init = Element{}) noexcept {
    for (size_t i = 0; i < size; ++i) {
      push_back(init);
    }
  }
  constexpr StaticVector(std::initializer_list<Element> values) noexcept {
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

  [[nodiscard]] constexpr auto operator[](size_t idx) noexcept -> reference {
    return *(m_data.data() + idx);
  }
  [[nodiscard]] constexpr auto operator[](size_t idx) const noexcept -> const_reference {
    return *(m_data.data() + idx);
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto size() const noexcept -> size_type { return m_size; }
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type { return CAPACITY; }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto begin() noexcept -> iterator { return m_data.data(); }
  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return m_data.data(); }
  [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator { return m_data.data(); }
  [[nodiscard]] constexpr auto end() noexcept -> iterator { return m_data.data() + m_size; }
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
    return m_data.data() + m_size;
  }
  [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator {
    return m_data.data() + m_size;
  }
};

#endif  // STATIC_VECTOR_HPP_
