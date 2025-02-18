#ifndef STATIC_VECTOR_HPP_
#define STATIC_VECTOR_HPP_

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "ReverseIterator.hpp"
#include "UninitializedArray.hpp"

// -------------------------------------------------------------------------------------------------
template <typename Element, size_t CAPACITY>
class StaticVector {
  detail::UninitializedArray<Element, CAPACITY> m_storage;
  size_t m_size = 0UZ;

 public:
  using value_type             = Element;
  using size_type              = size_t;
  using difference_type        = ssize_t;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using pointer                = value_type*;
  using const_pointer          = const value_type*;
  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = detail::ReverseIterator<Element>;
  using const_reverse_iterator = detail::ConstReverseIterator<Element>;

  static constexpr auto constructor_and_destructor_are_cheap =
      detail::UninitializedArray<Element, CAPACITY>::constructor_and_destructor_are_cheap;

  constexpr StaticVector() noexcept = default;
  constexpr StaticVector(size_t size, const Element& init = Element{}) noexcept {
    for (size_t i = 0; i < size; ++i) {
      push_back(init);
    }
  }
  constexpr StaticVector(std::initializer_list<Element> values) noexcept {
    for (auto& v : values) {
      push_back(std::move(v));
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
  constexpr auto operator=(const StaticVector& other) noexcept -> StaticVector& {
    if (this != &other) {
      clear();
      for (const auto& e : other) {
        push_back(e);
      }
    }
    return *this;
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr auto operator=(const StaticVector<OtherElement, OTHER_CAPACITY>& other) noexcept
      -> StaticVector& {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      assert(other.size() <= CAPACITY &&
             "Size of vector must be less than or equal to the capacity.");
    }

    clear();
    for (const auto& e : other) {
      push_back(Element{e});
    }
    return *this;
  }

  // - Move assignment -----------------------------------------------------------------------------
  constexpr auto operator=(StaticVector&& other) noexcept -> StaticVector& {
    if (this != &other) {
      clear();
      for (auto& e : other) {
        push_back(std::move(e));
      }
    }
    return *this;
  }

  template <typename OtherElement, size_t OTHER_CAPACITY>
  constexpr auto operator=(StaticVector<OtherElement, OTHER_CAPACITY>&& other) noexcept
      -> StaticVector& {
    if constexpr (OTHER_CAPACITY > CAPACITY) {
      assert(other.size() <= CAPACITY &&
             "Size of vector must be less than or equal to the capacity.");
    }

    clear();
    for (auto& e : other) {
      push_back(std::move(e));
    }
    return *this;
  }

  // -------------------------------------------------------------------------------------------------
  constexpr ~StaticVector() noexcept = default;
  constexpr ~StaticVector() noexcept
  requires(!std::is_trivially_destructible_v<Element>)
  {
    for (size_t i = 0; i < m_size; ++i) {
      std::destroy_at(m_storage.data() + i);
    }
  }

  // -------------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto operator[](size_t idx) noexcept -> reference {
    return *(m_storage.data() + idx);
  }
  [[nodiscard]] constexpr auto operator[](size_t idx) const noexcept -> const_reference {
    return *(m_storage.data() + idx);
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto data() noexcept -> pointer { return m_storage.data(); }
  [[nodiscard]] constexpr auto data() const noexcept -> const_pointer { return m_storage.data(); }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto empty() const noexcept -> bool { return m_size == 0UZ; }
  [[nodiscard]] constexpr auto size() const noexcept -> size_type { return m_size; }
  [[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return CAPACITY; }
  constexpr void reserve([[maybe_unused]] size_type reserve_capacity) const noexcept {
    assert(reserve_capacity <= CAPACITY && "Reserved capacity must be less than CAPACITY.");
  }
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type { return CAPACITY; }
  constexpr void shrink_to_fit() const noexcept { /* NOOP */ }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto begin() noexcept -> iterator { return m_storage.data(); }
  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return m_storage.data(); }
  [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator {
    return m_storage.data();
  }
  [[nodiscard]] constexpr auto end() noexcept -> iterator { return m_storage.data() + m_size; }
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
    return m_storage.data() + m_size;
  }
  [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator {
    return m_storage.data() + m_size;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator {
    return reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto crbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() + static_cast<difference_type>(m_size) - 1};
  }
  [[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator {
    return reverse_iterator{m_storage.data() - 1};
  }
  [[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() - 1};
  }
  [[nodiscard]] constexpr auto crend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator{m_storage.data() - 1};
  }

  // ------------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto front() noexcept -> reference {
    assert(m_size > 0UZ && "Vector must contain at least one element.");
    return operator[](0UZ);
  }
  [[nodiscard]] constexpr auto front() const noexcept -> const_reference {
    assert(m_size > 0UZ && "Vector must contain at least one element.");
    return operator[](0UZ);
  }
  [[nodiscard]] constexpr auto back() noexcept -> reference {
    assert(m_size > 0UZ && "Vector must contain at least one element.");
    return operator[](m_size - 1UZ);
  }
  [[nodiscard]] constexpr auto back() const noexcept -> const_reference {
    assert(m_size > 0UZ && "Vector must contain at least one element.");
    return operator[](m_size - 1UZ);
  }

  // -------------------------------------------------------------------------------------------------
  constexpr void clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<Element>) {
      for (size_t i = 0; i < m_size; ++i) {
        std::destroy_at(m_storage.data() + i);
      }
    }
    m_size = 0UZ;
  }

  // -------------------------------------------------------------------------------------------------
  constexpr void push_back(const Element& e) noexcept {
    assert(m_size < CAPACITY && "Size may not exceed capacity.");
    std::construct_at(m_storage.data() + m_size, e);
    m_size += 1;
  }
  constexpr void push_back(Element&& e) noexcept {
    assert(m_size < CAPACITY && "Size may not exceed capacity.");
    std::construct_at(m_storage.data() + m_size, std::move(e));
    m_size += 1;
  }

  // -------------------------------------------------------------------------------------------------
  template <typename... Args>
  constexpr void emplace_back(Args&&... args) noexcept {
    assert(m_size < CAPACITY && "Size may not exceed capacity.");
    std::construct_at(m_storage.data() + m_size, std::forward<Args>(args)...);
    m_size += 1;
  }

  // -------------------------------------------------------------------------------------------------
  constexpr auto pop_back() noexcept -> value_type {
    assert(m_size > 0 && "Vector cannot be empty.");
    m_size -= 1;
    auto tmp = std::move(operator[](m_size));
    std::destroy_at(m_storage.data() + m_size);
    return tmp;
  }

  // -------------------------------------------------------------------------------------------------
  // TODO:
  // - insert
  // - insert_range
  // - emplace
  // - erase
  // - append_range
  // - resize
  // - swap
};

#endif  // STATIC_VECTOR_HPP_
