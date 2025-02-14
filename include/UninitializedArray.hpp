#ifndef UNINITIALIZED_ARRAY_HPP_
#define UNINITIALIZED_ARRAY_HPP_

#include <cstddef>
#include <type_traits>

namespace detail {

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

}  // namespace detail

#endif  // UNINITIALIZED_ARRAY_HPP_
