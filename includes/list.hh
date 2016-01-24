#ifndef _META_LIST_H
#define _META_LIST_H  1

#include <type_traits>
#include "type_wrappers.hh"
#include "function.hh"

namespace meta { namespace list {

using namespace meta::typewrappers;
using namespace meta::function;

struct empty {
  using type = empty;
};

template <typename h, typename t>
struct cons {
  using type = cons<h, t>;
  using head = h;
  using tail = t;
};

struct cdr_f {
  template <typename cons>
  struct apply {
    using type = typename cons::type::tail::type;
  };
};

template <typename cons>
struct cdr : cdr_f::template apply<cons> {};

template <typename cons>
struct cddr : cdr_f::template apply<typename cdr<cons>::type> {};

struct car_f {
  template <typename cons>
  struct apply {
    using type = typename cons::type::head::type;
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

  template <typename fn>
  struct apply<fn, empty> : empty {};
};

template <typename fn, typename list>
struct map : map_f::template apply<fn, list> {};

struct foldr_f {
  template <typename fn, typename base, typename list>
  struct apply : fn::template apply<
    typename car<list>::type,
    typename foldr_f::template apply<fn, base, typename cdr<list>::type>::type
  > {};

  template <typename fn, typename base>
  struct apply<fn, base, empty> : base {};
};

template <typename fn, typename base, typename list>
struct foldr : foldr_f::template apply<fn, base, list> {};

struct foldl_f {
  template <typename fn, typename acc, typename list>
  struct apply : foldl_f::template apply<
    fn,
    typename fn::template apply<typename car<list>::type, acc>::type,
    typename cdr<list>::type
  > {};

  template <typename fn, typename acc>
  struct apply<fn, acc, empty> : acc {};
};

template <typename fn, typename acc, typename list>
struct foldl : foldl_f::template apply<fn, acc, list> {};

struct foreach_f {
  template <typename fn, typename list>
  struct apply : fn::template apply<typename car<list>::type>,
    foreach_f::template apply<fn, typename cdr<list>::type> {};

  template <typename fn>
  struct apply<fn, empty> : void_ {};
};

template <typename fn, typename list>
struct foreach : foreach_f::template apply<fn, list> {};

struct filter_f {
  template <typename pred, typename list>
  struct apply : std::conditional<
    pred::template apply<typename car<list>::type>::type::value,
    cons<
      typename car<list>::type,
      typename filter_f::template apply<pred, typename cdr<list>::type>
    >,
    typename filter_f::template apply<pred, typename cdr<list>::type>::type
  > {};

  template <typename pred>
  struct apply<pred, empty> : empty {};
};

template <typename pred, typename list>
struct filter : filter_f::template apply<pred, list> {};

struct transform_f {
  template <typename list1, typename list2, typename fn>
  struct apply : cons<
    typename fn::template apply<typename car<list1>::type, typename car<list2>::type>::type,
    typename transform_f::template apply<
      typename cdr<list1>::type,
      typename cdr<list2>::type,
      fn
    >::type
  > {};

  template <typename list1, typename fn>
  struct apply<list1, empty, fn> : cons<
    typename fn::template apply<typename car<list1>::type, empty>,
    typename transform_f::template apply<
      typename cdr<list1>::type,
      empty,
      fn
    >::type
  > {};

  template <typename list2, typename fn>
  struct apply<empty, list2, fn> : cons<
    typename fn::template apply<empty, typename car<list2>::type>::type,
    typename transform_f::template apply<
      empty,
      typename cdr<list2>::type,
      fn
    >::type
  > {};

  template <typename fn>
  struct apply<empty, empty, fn> : empty {};
};

template <typename list1, typename list2, typename fn>
struct transform : transform_f::template apply<list1, list2, fn> {};

struct equal_f {
  template <typename list1, typename list2, typename pred = lambda<std::is_same>>
  struct apply : std::conditional<
    !pred::template apply<
      typename car<list1>::type,
      typename car<list2>::type
    >::type::value,
    bool_<false>,
    typename equal_f::template apply<
      typename cdr<list1>::type,
      typename cdr<list2>::type,
      pred
    >::type
  > {};

  template <typename list, typename pred>
  struct apply<empty, list, pred> : bool_<false> {};

  template <typename list, typename pred>
  struct apply<list, empty, pred> : bool_<false> {};

  template <typename pred>
  struct apply<empty, empty, pred> : bool_<true> {};
};

template <typename list1, typename list2, typename pred = lambda<std::is_same>>
struct equal : equal_f::template apply<list1, list2, pred> {};

}}

#endif
