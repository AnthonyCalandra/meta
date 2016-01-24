#ifndef _META_REFLECT_H
#define _META_REFLECT_H 1

#include <type_traits>
#include "type_wrappers.hh"

namespace meta { namespace reflect {

// TODO: build off std::type_traits

// These macros create indirection when concatenating tokens.
#define __META_TOKENPASTE(x, y) x ## y
#define _META_TOKENPASTE(x, y) __META_TOKENPASTE(x, y)

#define META_MAKE_FRIEND(klass, member) \
friend class _META_GET_CLASS_HAS_MEMBER_NAME(member)<_META_TOKENPASTE(klass,)>

#define _META_GET_CLASS_HAS_MEMBER_NAME(member) class_has_member_##member

#define META_DECLARE_CLASS_MEMBER(member) \
template <typename T> \
struct _META_GET_CLASS_HAS_MEMBER_NAME(member) { \
  template <typename C> \
  static std::true_type has(decltype(&C::_META_TOKENPASTE(member,))); \
  template <typename C> \
  static std::false_type has(...); \
  using type = bool_<std::is_same<decltype(has<T>(0)), std::true_type>::type::value>; \
}

#define META_GET_METHOD_TYPE(klass, method) \
decltype(std::declval<_META_TOKENPASTE(klass,)>()._META_TOKENPASTE(method,)())

#define META_GET_FIELD_TYPE(klass, field) \
decltype(std::declval<_META_TOKENPASTE(klass,)>()._META_TOKENPASTE(field,))

}}

#endif
