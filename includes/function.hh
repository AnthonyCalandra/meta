#ifndef _META_FUNCTION_H
#define _META_FUNCTION_H  1

namespace meta { namespace function {

  struct apply_f {
    template <template <typename...> class F, typename ...args>
    struct apply : F<typename args::type...> {};
  };

  template <template <typename...> class F, typename ...args>
  struct apply : apply_f::template apply<F, args...> {};

  template <template <typename...> class F>
  struct lambda {
    template <typename ...args>
    struct apply : apply_f::template apply<F, args...> {};
  };

}}

#endif
