#ifndef MINESWEEPER_TYPE_TRAITS_H
#define MINESWEEPER_TYPE_TRAITS_H

#include <type_traits> // std::enable_if, std::integral_constant

namespace minesweeper {

// basically same as std::void_t
// included here so we do not need c++17
template <typename...>
using void_t = void;

// template helper function to decide if type is an expandable sequence container
// (eg. std::vector, std::string, std::deque, std::list)
template <typename, typename = std::void_t<>>
struct is_expandable_sequence_container : std::false_type {};

template <typename T>
struct is_expandable_sequence_container<
    T, void_t<decltype(std::declval<T>().size()),
              decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))>> : std::true_type {};

// template helper function to decide if type is an expandable 1D sequence container
// (eg. std::vector<int>, std::string, std::deque<int>, std::list<int>)
template <typename, typename = void>
struct is_expandable_1d_sequence_container : std::false_type {};
template <typename T>
struct is_expandable_1d_sequence_container<
    T, typename std::enable_if<is_expandable_sequence_container<T>::value &&
                               !is_expandable_sequence_container<typename T::value_type>::value>::type>
    : std::true_type {};

// template helper function to decide if type is an expandable 2D sequence container
// (eg. std::vector<std::vector<int>>, std::vector<std::string>, etc.)
template <typename, typename = void>
struct is_expandable_2d_sequence_container : std::false_type {};
template <typename T>
struct is_expandable_2d_sequence_container<
    T, typename std::enable_if<is_expandable_sequence_container<T>::value &&
                               is_expandable_1d_sequence_container<typename T::value_type>::value>::type>
    : std::true_type {};

} // namespace minesweeper

#endif // MINESWEEPER_TYPE_TRAITS_H