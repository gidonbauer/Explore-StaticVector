#ifndef REVERSE_ITERATOR_HPP_
#define REVERSE_ITERATOR_HPP_

#include <cassert>
#include <iterator>

namespace detail {

// =================================================================================================
template <typename Element>
class ReverseIterator {
  Element* m_ptr = nullptr;

 public:
  using difference_type = ssize_t;
  using value_type      = Element;
  using element_type    = Element;
  using pointer         = Element*;
  using reference       = Element&;

  constexpr ReverseIterator() noexcept = default;
  constexpr ReverseIterator(Element* ptr) noexcept
      : m_ptr(ptr) {}

  constexpr auto operator==(const ReverseIterator<Element>& other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  constexpr auto operator<=>(const ReverseIterator<Element>& other) const noexcept {
    return m_ptr <=> other.m_ptr;
  }

  constexpr auto operator*() noexcept -> reference {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator*() const noexcept -> reference {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator->() noexcept -> pointer {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator->() const noexcept -> pointer {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator[](difference_type offset) noexcept -> reference {
    return *(m_ptr - offset);
  }
  constexpr auto operator[](difference_type offset) const noexcept -> reference {
    return *(m_ptr - offset);
  }

  constexpr auto operator++() noexcept -> ReverseIterator& {
    m_ptr -= 1;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> ReverseIterator {
    const auto res = *this;
    m_ptr -= 1;
    return res;
  }

  constexpr auto operator--() noexcept -> ReverseIterator& {
    m_ptr += 1;
    return *this;
  }
  constexpr auto operator--(int) noexcept -> ReverseIterator {
    const auto res = *this;
    m_ptr += 1;
    return res;
  }

  constexpr auto operator+(difference_type offset) const noexcept -> ReverseIterator {
    return ReverseIterator{m_ptr - offset};
  }
  constexpr auto operator+=(difference_type offset) noexcept -> ReverseIterator& {
    m_ptr -= offset;
    return *this;
  }

  friend constexpr auto operator+(difference_type offset, ReverseIterator iter) noexcept
      -> ReverseIterator {
    iter.m_ptr -= offset;
    return iter;
  }

  constexpr auto operator-(difference_type offset) const noexcept -> ReverseIterator {
    return ReverseIterator{m_ptr + offset};
  }
  constexpr auto operator-=(difference_type offset) noexcept -> ReverseIterator& {
    m_ptr += offset;
    return *this;
  }

  constexpr auto operator-(const ReverseIterator& other) const noexcept -> difference_type {
    return other.m_ptr - m_ptr;
  }
};

// =================================================================================================
template <typename Element>
class ConstReverseIterator {
  const Element* m_ptr = nullptr;

 public:
  using difference_type = ssize_t;
  using value_type      = Element;
  using element_type    = const Element;
  using pointer         = const Element*;
  using reference       = const Element&;

  constexpr ConstReverseIterator() noexcept = default;
  constexpr ConstReverseIterator(const Element* ptr) noexcept
      : m_ptr(ptr) {}

  constexpr auto operator==(const ConstReverseIterator<Element>& other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  constexpr auto operator<=>(const ConstReverseIterator<Element>& other) const noexcept {
    return m_ptr <=> other.m_ptr;
  }

  constexpr auto operator*() const noexcept -> reference {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return *m_ptr;
  }
  constexpr auto operator->() const noexcept -> pointer {
    assert(m_ptr != nullptr && "ReverseIterator cannot point to nullptr.");
    return m_ptr;
  }
  constexpr auto operator[](difference_type offset) const noexcept -> reference {
    return *(m_ptr - offset);
  }

  constexpr auto operator++() noexcept -> ConstReverseIterator& {
    m_ptr -= 1;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> ConstReverseIterator {
    const auto res = *this;
    m_ptr -= 1;
    return res;
  }

  constexpr auto operator--() noexcept -> ConstReverseIterator& {
    m_ptr += 1;
    return *this;
  }
  constexpr auto operator--(int) noexcept -> ConstReverseIterator {
    const auto res = *this;
    m_ptr += 1;
    return res;
  }

  constexpr auto operator+(difference_type offset) const noexcept -> ConstReverseIterator {
    return ReverseIterator{m_ptr - offset};
  }
  constexpr auto operator+=(difference_type offset) noexcept -> ConstReverseIterator& {
    m_ptr -= offset;
    return *this;
  }
  friend constexpr auto operator+(difference_type offset, ConstReverseIterator iter) noexcept
      -> ConstReverseIterator {
    iter.m_ptr -= offset;
    return iter;
  }

  constexpr auto operator-(difference_type offset) const noexcept -> ConstReverseIterator {
    return ReverseIterator{m_ptr + offset};
  }
  constexpr auto operator-=(difference_type offset) noexcept -> ConstReverseIterator& {
    m_ptr += offset;
    return *this;
  }

  constexpr auto operator-(const ConstReverseIterator& other) const noexcept -> difference_type {
    return other.m_ptr - m_ptr;
  }
};

}  // namespace detail

// =================================================================================================
template <typename Element>
struct std::iterator_traits<detail::ReverseIterator<Element>> {
  using difference_type   = detail::ReverseIterator<Element>::difference_type;
  using value_type        = detail::ReverseIterator<Element>::value_type;
  using pointer           = detail::ReverseIterator<Element>::pointer;
  using reference         = detail::ReverseIterator<Element>::reference;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename Element>
struct std::iterator_traits<detail::ConstReverseIterator<Element>> {
  using difference_type   = detail::ConstReverseIterator<Element>::difference_type;
  using value_type        = detail::ConstReverseIterator<Element>::value_type;
  using pointer           = detail::ConstReverseIterator<Element>::pointer;
  using reference         = detail::ConstReverseIterator<Element>::reference;
  using iterator_category = std::random_access_iterator_tag;
};

#endif  // REVERSE_ITERATOR_HPP_
