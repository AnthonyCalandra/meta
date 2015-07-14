#ifndef _META_LIST_H
#define _META_LIST_H  1

#include <type_traits>
#include "type_wrappers.hpp"

namespace meta { namespace list {

using namespace meta::typewrappers;

struct empty {
  using type = empty;
  using value = empty;
};

template <typename car, typename cdr>
struct cons {
  using type = cons<car, cdr>;
  using value = car;
  using next = cdr;
};

struct cdr_f {
  template <typename cons>
  struct apply {
    using type = typename cons::type::next::type;
  };
};

template <typename cons>
struct cdr : cdr_f::template apply<cons> {};

template <typename cons>
struct cddr : cdr_f::template apply<typename cdr<cons>::type> {};

struct car_f {
  template <typename cons>
  struct apply {
    using type = typename cons::type::value;
  };
};

template <typename cons>
struct car : car_f::template apply<cons> {};

template <typename cons>
struct caar : car_f::template apply<typename car<cons>::type> {};

template <typename first = empty, typename ...rest>
struct list : std::conditional<
  sizeof...(rest) == 0,
  cons<first, empty>,
  cons<first, typename list<rest...>::type>
>::type {};

template <>
struct list<empty> : empty {};

template <typename T, T ...elements>
struct list_c : list<type_<T, elements>...> {};

struct length_f {
  template <typename list>
  struct apply : std::conditional<
    std::is_same<typename list::type, empty>::value,
    int_<0>,
    plus_f::template apply<int_<1>, length_f::template apply<cdr<list>>>
  > {};
};

template <typename list>
struct length : length_f::template apply<list> {};

struct map_f {
  template <typename fn, typename list>
  struct apply : cons<
    typename fn::template apply<typename car<list>::type>,
    map_f::template apply<fn, typename cdr<list>::type>
  > {};
};

template <typename fn, typename list>
struct map : map_f::template apply<fn, list> {};

}}

#endif
