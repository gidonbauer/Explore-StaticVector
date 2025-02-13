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
  static constexpr bool constructor_and_destructor_are_cheap =
      std::is_trivially_default_constructible_v<Element> &&
      std::is_trivially_destructible_v<Element>;

  using Storage_t = std::conditional_t<constructor_and_destructor_are_cheap,
                                       Element[CAPACITY],                       // NOLINT
                                       std::byte[CAPACITY * sizeof(Element)]>;  // NOLINT
  alignas(Element) Storage_t m_data;

  [[nodiscard]] constexpr auto data() noexcept -> Element* {
    if constexpr (constructor_and_destructor_are_cheap) {
      return m_data;
    } else {
      return reinterpret_cast<Element*>(m_data);
    }
  }
  [[nodiscard]] constexpr auto data() const noexcept -> const Element* {
    if constexpr (constructor_and_destructor_are_cheap) {
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
  using difference_type = ssize_t;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = value_type*;
  using const_pointer   = const value_type*;
  using iterator        = pointer;
  using const_iterator  = const_pointer;
  // using reverse_iterator       = void;
  // using const_reverse_iterator = void;

  static constexpr auto constructor_and_destructor_are_cheap =
      UninitializedArray<Element, CAPACITY>::constructor_and_destructor_are_cheap;

  constexpr StaticVector() noexcept = default;
  constexpr StaticVector(size_t size, const Element& init = Element{}) noexcept {
    for (size_t i = 0; i < size; ++i) {
      push_back(init);
    }
  }
  constexpr StaticVector(std::initializer_list<Element> values) noexcept {
    for (auto& v : values) {
      push_back(v);
    }
  }

  // - Copy constructor ----------------------------------------------------------------------------
  constexpr StaticVector(const StaticVector& other) noexcept {
    for (const auto& e : other) {
      push_back(e);
    }
  }
  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr StaticVector(const StaticVector<OtherElement, OTHER_CAPACITY>& other) noexcept {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      assert(other.size() <= CAPACITY &&
             "Size of vector must be less than or equal to the capacity.");
    }

    for (const auto& e : other) {
      push_back(Element{e});
    }
  }

  // - Move constructor ----------------------------------------------------------------------------
  constexpr StaticVector(StaticVector&& other) noexcept {
    for (auto& e : other) {
      push_back(std::move(e));
    }
  }
  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr StaticVector(StaticVector<OtherElement, OTHER_CAPACITY>&& other) noexcept {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      assert(other.size() <= CAPACITY &&
             "Size of vector must be less than or equal to the capacity.");
    }

    for (auto& e : other) {
      push_back(Element{std::move(e)});
    }
  }

  // - Copy assignment -----------------------------------------------------------------------------
  constexpr auto operator=(const StaticVector& other) noexcept -> StaticVector& = delete;
  // - Move assignment -----------------------------------------------------------------------------
  constexpr auto operator=(StaticVector&& other) noexcept -> StaticVector& = delete;

  constexpr ~StaticVector() noexcept = default;
  constexpr ~StaticVector() noexcept
  requires(!std::is_trivially_destructible_v<Element>)
  {
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

  constexpr auto pop_back() noexcept -> value_type {
    assert(m_size > 0 && "Vector cannot be empty.");
    m_size -= 1;
    auto tmp = std::move(operator[](m_size));
    std::destroy_at(m_data.data() + m_size);
    return tmp;
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
