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
  };

  template<typename P, typename Env>
  struct xform_placeholder<P, Env, false> {
    typedef P type;
  };

  template<typename T, typename Env>
  struct xform_placeholder_tuple {};

  template<typename... _T, typename Env>
  struct xform_placeholder_tuple<thrust::tuple<_T...>, Env> {
    typedef thrust::tuple<typename xform_placeholder<_T, Env>::type...> type;

  };

  template<typename T, typename Env>
  struct tuple_apply_if_placeholder {
  };

  template<typename... _T, typename Env>
  struct tuple_apply_if_placeholder<thrust::tuple<_T...>, Env> {
  };

  template<typename T, typename Env>
  //typename detail::xform_placeholder_tuple<T, Env>::type
  void
  replace_placeholders(T formal, Env actual) {
    //return tuple_apply_if_placeholder<T, Env>()(formal, actual);
  }
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
      detail::replace_placeholders(m_args, thrust::make_tuple(args...));
      //return mpl::apply_from_tuple(m_fn, __replace_placeholders<_Args...>(std::forward<_Args>(args)...));
    }

  private:
    typename std::decay<F>::type m_fn;
    thrust::tuple<typename std::decay<Args>::type...> m_args;
  };

  template<typename F, typename... Args>
  __host__ __device__
  __bind<F, Args...>
  bind(F&& f, Args&&... args) {
    return __bind<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
  }
} // namespace cb
