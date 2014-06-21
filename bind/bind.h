#pragma once

#include <type_traits>
#include <functional>
#include <thrust/tuple.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/tuple_meta_transform.h>

#include "mpl.h"
#include "placeholders.h"

namespace cb {
namespace detail {

  template<typename P, typename Env,
           bool _is_placeholder = placeholders::is_placeholder<P>::value>
  struct xform_placeholder {
    typedef typename thrust::tuple_element<P::value, Env>::type type;

    __host__ __device__
    static auto apply(P&& arg, Env env) -> decltype(arg(env))
    { return arg(env); }
  };

  template<typename P, typename Env>
  struct xform_placeholder<P, Env, false> {
    typedef P type;

    __host__ __device__
    static P apply(P&& arg, Env env)
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

      /*return mpl::tuple_cat(xform_placeholder<HT, Env>::apply(formal.get_head(), actual),
                            xform_placeholder_tuple<TT, Env>::apply(formal.get_tail(), actual));*/
    }
  };
}

  template<typename F, typename... Args>
  struct __bind {
    typedef thrust::tuple<typename std::decay<Args>::type...> args_tuple_type;

    __host__ __device__
    __bind(F&& f, Args&&... args) :
      m_fn(f), m_args(args...) {}

    template<typename... _Args>
    __host__ __device__ typename F::result_type
    operator()(_Args&&... args) {
      //detail::replace_placeholders(m_args, thrust::make_tuple(args...));
      //return mpl::apply_from_tuple(m_fn, __replace_placeholders<_Args...>(std::forward<_Args>(args)...));

      typedef thrust::tuple<typename std::decay<_Args>::type...> _args_tuple_type;
      mpl::tuple_cat<thrust::tuple<int>, thrust::tuple<int>>(thrust::make_tuple(1), thrust::make_tuple(2));
      /*mpl::apply_from_tuple(
          m_fn, thrust::make_tuple(2, 2));*/
          /*detail::xform_placeholder_tuple<_args_tuple_type, typename std::decay<Args>::type...>::
          apply(m_args, thrust::make_tuple(args...)));*/
    }

  private:
    typename std::decay<F>::type m_fn;
    args_tuple_type m_args;
  };

  template<typename F, typename... Args>
  __host__ __device__
  __bind<F, Args...>
  bind(F&& f, Args&&... args) {
    return __bind<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
  }
} // namespace cb
