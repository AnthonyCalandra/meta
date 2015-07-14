#ifndef _META_TYPE_WRAPPERS_H
#define _META_TYPE_WRAPPERS_H 1

#include <type_traits>

namespace meta { namespace typewrappers {

  template <typename T, T N>
  struct type_ {
    using type = type_<T, N>;
    using value_type = T;
    static constexpr T value = N;
    using next = type_<T, N + 1>;
    using prev = type_<T, N - 1>;
  };

  template <bool B>
  struct bool_;

  template <>
  struct bool_<true> : std::true_type {};

  template <>
  struct bool_<false> : std::false_type {};

  template <unsigned char N>
  struct uchar_ : type_<unsigned char, N> {
    using type = uchar_<N>;
  };

  template <char N>
  struct char_ : type_<char, N> {
    using type = char_<N>;
  };

  template <unsigned int N>
  struct uint_ : type_<unsigned int, N> {
    using type = uint_<N>;
  };

  template <int N>
  struct int_ : type_<int, N> {
    using type = int_<N>;
  };

  template <unsigned long N>
  struct ulong_ : type_<unsigned long, N> {
    using type = ulong_<N>;
  };

  template <long N>
  struct long_ : type_<long, N> {
    using type = long_<N>;
  };

  template <typename T>
  struct next {
    using type = typename T::type::next::type;
  };

  template <typename T>
  struct prev {
    using type = typename T::type::prev::type;
  };

  struct plus_f {
    template <typename T, typename N>
    struct apply : type_<typename T::value_type, T::type::value + N::type::value> {};
  };

  template <typename T, typename N>
  struct plus : plus_f::template apply<T, N> {};

  struct minus_f {
    template <typename T, typename N>
    struct apply : type_<typename T::value_type, T::type::value - N::type::value> {};
  };

  template <typename T, typename N>
  struct minus : minus_f::template apply<T, N> {};

  struct multiply_f {
    template <typename T, typename N>
    struct apply : type_<typename T::value_type, T::type::value * N::type::value> {};
  };

  template <typename T, typename N>
  struct multiply : multiply_f::template apply<T, N> {};

  struct divide_f {
    template <typename T, typename N>
    struct apply : type_<typename T::value_type, T::type::value / N::type::value> {};
  };

  template <typename T, typename N>
  struct divide : divide_f::template apply<T, N> {};

  struct modulo_f {
    template <typename T, typename N>
    struct apply : type_<typename T::value_type, T::type::value % N::type::value> {};
  };

  template <typename T, typename N>
  struct modulo : modulo_f::template apply<T, N> {};

}}

#endif
