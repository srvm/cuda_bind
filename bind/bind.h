#pragma once

#include <type_traits>
#include <functional>
#include <thrust/tuple.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/tuple_meta_transform.h>
#include <thrust/detail/tuple_cat.h>

#include "mpl.h"
#include "placeholders.h"

namespace cb {
namespace detail {

  template<typename P, typename Env,
           bool _is_placeholder = placeholders::is_placeholder<P>::value>
  struct xform_placeholder {
    static_assert(P::value < thrust::tuple_size<Env>::value,
        "Error: Placeholder does not point to anything");

    typedef typename thrust::tuple_element<P::value, Env>::type type;

    __host__ __device__
    static type apply(P arg, Env env)
    { return arg(env); }
  };

  template<typename P, typename Env>
  struct xform_placeholder<P, Env, false> {
    typedef P type;

    __host__ __device__
    static P apply(P arg, Env env)
    { return arg; }
  };

  template<typename Env, typename... T>
  struct xform_placeholder_tuple {
    typedef thrust::tuple<typename xform_placeholder<T, Env>::type...> type;

    __host__ __device__
    static type apply(thrust::tuple<T...> formal, Env actual) {
      typedef thrust::tuple<T...> arg_type;
      typedef typename arg_type::head_type HT;
      typedef typename arg_type::tail_type TT;

      auto head = thrust::make_tuple(xform_placeholder<HT, Env>::apply(formal.get_head(), actual));
      auto tail = xform_placeholder_tuple<Env, TT>::apply(formal.get_tail(), actual);

      return thrust::tuple_cat(head, tail);
    }
  };

  template<typename Env, typename T>
  struct xform_placeholder_tuple<Env, thrust::detail::cons<T, thrust::null_type>> {
    typedef thrust::tuple<typename xform_placeholder<T, Env>::type> type;

    __host__ __device__
    static type apply(thrust::detail::cons<T, thrust::null_type> formal, Env actual)
    { return thrust::make_tuple(xform_placeholder<T, Env>::apply(formal.get_head(), actual)); }
  };
}

  template<typename R, typename F, typename... Args>
  struct __bind {
    typedef typename std::decay<F>::type F_decay;
    typedef thrust::tuple<typename std::decay<Args>::type...> args_tuple_type;

    __host__ __device__
    __bind(F&& f, Args&&... args) :
      m_fn(f), m_args(args...) {}

    template<typename... _Args>
    __host__ __device__
    R operator()(_Args&&... args) {
      typedef thrust::tuple<typename std::decay<_Args>::type...> _args_tuple_type;

      auto value_tuple = detail::xform_placeholder_tuple<_args_tuple_type,
           typename std::decay<Args>::type...>::apply(
               m_args, thrust::make_tuple(std::forward<_Args>(args)...));

      return mpl::apply_from_tuple<R>(m_fn, value_tuple);
    }

    __host__ __device__
    R operator()() {
      static_assert(!placeholders::has_placeholder<args_tuple_type>::value,
          "One or more placeholders could not be bound");

      return mpl::apply_from_tuple<R>(m_fn, m_args);
    }

  private:
    F_decay m_fn;
    args_tuple_type m_args;
  };

  template<typename F, typename... Args>
  __host__ __device__
  __bind<typename F::result_type, F, Args...>
  bind(F&& f, Args&&... args) {
    return __bind<typename F::result_type, F, Args...>
      (std::forward<F>(f), std::forward<Args>(args)...);
  }

  template<typename R, typename F, typename... Args>
  __host__ __device__
  __bind<R, F, Args...> bind(F&& f, Args&&... args) {
    return __bind<R, F, Args...>
      (std::forward<F>(f), std::forward<Args>(args)...);
  }
} // namespace cb
