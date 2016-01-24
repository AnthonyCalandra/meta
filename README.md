# meta

meta is a C++11 library built on top of the type_traits standard library focused
on functional-style metaprogramming.

## Type Wrappers

__Type wrappers__ (otherwise known as __integral constant wrappers__) provide wrappers for
manipulating fundamental, arithmetic C++ types and their values for use in template
metaprograms.

### Example

```cpp
#include <iostream>
#include "meta.hh"

using namespace meta::typewrappers;

int main() {
  std::cout << plus<uint_<1>, uint_<1>>::value << std::endl; // == 2
  return 0;
}
```
Currently the following operations are supported:
* addition, subtraction, multiplication, division, modulo
* next, prev

The following type wrappers are implemented:
* A generic `type_` type.
* `bool_` (wrapper for `std::true_type` and `std::false_type`).
* `char_`, `uchar_`.
* `int_`, `uint_`.
* `long_`, `ulong_`.

## Lambda and apply

`apply` is a metafunction that accepts another metafunction and applies it with the arguments you give it:
```cpp
template <typename x, typename y>
struct sum_f : int_<x::value + y::value> {};
apply<sum_f, int_<1>, int_<1>>::type::value; // == 2
```
Using `lambda` you can turn metafunctions into metafunction classes and apply them as such:
```cpp
template <typename x, typename y>
struct sum_f : int_<x::value + y::value> {};
lambda<sum_f>::template apply<int_<1>, int_<1>>::type::value; // == 2
```

## Lists

Lists are fundamentally designed in the same way as in Scheme, by pairing a value and a list using `cons` to produce a list:
```scheme
(cons 1 (cons 2 (cons 3 '())))
```
in meta:
```cpp
cons<int_<1>, cons<int_<2>, cons<int_<3>, empty>>>
```
car and cdr operations are also defined in the same way as in Scheme:
```cpp
car<cons<int_<1>, empty>>::type::value // == 1
cdr<cons<int_<1>, cons<int_<2>, int_<3>>>>::type // == cons<int_<2>, int_<3>>
```
Since using cons creates a lot of syntactic noise, like in Scheme, there is also a `list` metafunction:
```cpp
list<int_<1>, int_<2>, int_<3>> // == cons<int_<1>, cons<int_<2>, int_<3>>>
```
Still, this ends up producing a lot of noise because of the integral constant wrappers, so `list_c` was invented:
```cpp
list_c<int, 1, 2, 3> // == list<int_<1>, int_<2>, int_<3>>
```
You can also operate on these lists, with a little help from `lambda`, incrementing each value in a list is easy:
```cpp
map<lambda<incr_f>, list_c<int, 1, 2, 3>> // == list_c<int, 2, 3, 4>
```

## Reflection

meta currently has limited support for reflection. Due to the way template metaprogramming works,
it's not possible to do much without introducing many macros into the code, but with `std:;type_traits` this makes some things now possible. Like the library, this section is a work in progress.

Here's an example of determining if a class has a `foo` member:
```cpp
META_DECLARE_CLASS_MEMBER(foo);
class FooFieldClass {
public:
  int foo;
};
class FooMethodClass {
public:
  void foo() {}
};
class EmptyClass {};
class_has_member_foo<EmptyClass>::type::value // == false
class_has_member_foo<FooFieldClass>::type::value // == true
class_has_member_foo<FooMethodClass>::type::value // == true
```
Unfortunately this isn't perfect since private and protected members return false. However, you can use `META_MAKE_FRIEND` to allow access to all members:
```cpp
META_DECLARE_CLASS_MEMBER(foo);
class FooClass {
  META_MAKE_FRIEND(FooClass, foo);
  void foo() {}
};
class_has_member_foo<FooClass>::type::value // == true
```

With meta you can also get types of methods and fields:
```cpp
class FooClass {
public:
  int foo;
  void bar() {}
};
META_GET_FIELD_TYPE(FooClass, foo); // == int
META_GET_METHOD_TYPE(FooClass, bar); // == void
```

# License

MIT

# Author

Anthony Calandra
