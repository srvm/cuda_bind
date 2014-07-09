#pragma once

#include <functional>

#include <thrust/tuple.h>
#include <thrust/detail/tuple_cat.h>

#include "mpl.h"
#include "placeholders.h"

namespace cb {
// XXX: This throws a compile error due to a bug in EDG.
// #define __VTT(T) thrust::tuple<typename std::decay<T>::type...>

// Create a thrust::tuple type from variadic types using decltype.
#define __VTT(T, x) decltype(thrust::make_tuple(std::forward<T>(x)...))

namespace detail {

  template<typename P, typename Env,
           bool _is_placeholder = placeholders::is_placeholder<P>::value>
  struct xform_placeholder {
    //typedef typename thrust::tuple_element<P::value, Env>::type type;
    typedef typename P::template result_type<Env> type;

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

  template<typename T, typename Env,
           int size = thrust::tuple_size<T>::value>
  struct xform_placeholder_tuple {
    template<typename _T> using xform_bound = xform_placeholder<_T, Env>;
    typedef typename thrust::detail::tuple_meta_transform<T, xform_bound>::type type;

    __host__ __device__
    static type apply(T formal, Env actual) {
      typedef typename T::head_type HT;
      typedef typename T::tail_type TT;

      auto head = thrust::make_tuple(xform_placeholder<HT, Env>::apply(formal.get_head(), actual));
      auto tail = xform_placeholder_tuple<TT, Env>::apply(formal.get_tail(), actual);

      return thrust::tuple_cat(head, tail);
    }
  };

  /*template<typename _T, typename Env>
  struct xform_placeholder_tuple<thrust::detail::cons<_T, thrust::null_type>, Env> {
    typedef thrust::tuple<typename xform_placeholder<_T, Env>::type> type;

    __host__ __device__
    static type apply(thrust::detail::cons<_T, thrust::null_type> formal, Env actual)
    { return thrust::make_tuple(xform_placeholder<_T, Env>::apply(formal.get_head(), actual)); }
  };*/

  template<typename T, typename Env>
  struct xform_placeholder_tuple<T, Env, 1> {
    typedef typename thrust::tuple_element<0, T>::type _T;
    typedef thrust::tuple<typename xform_placeholder<_T, Env>::type> type;

    __host__ __device__
    static type apply(T formal, Env actual)
    { return thrust::make_tuple(xform_placeholder<_T, Env>::apply(thrust::get<0>(formal), actual)); }
  };
}

  template<typename R, typename F, typename T>
  struct __bind {
    typedef typename std::decay<F>::type F_decay;

    __host__ __device__
    __bind(F&& f, T args) :
      m_fn(f), m_args(args) {}

    template<typename... _Args>
    __host__ __device__
    R operator()(_Args&&... args) {

      auto value_tuple = detail::xform_placeholder_tuple<T, __VTT(_Args, args)>::
        apply(m_args, thrust::make_tuple(std::forward<_Args>(args)...));

      return mpl::apply_from_tuple<R>(m_fn, value_tuple);
    }

    __host__ __device__
    R operator()() {
      static_assert(!placeholders::has_placeholder<T>::value,
          "One or more placeholders could not be bound");

      return mpl::apply_from_tuple<R>(m_fn, m_args);
    }

  private:
    F_decay m_fn;
    T m_args;
  };

  // Function pointers
  template<typename R, typename... F_Args, typename... Args>
  __host__ __device__
  auto bind(R (*f)(F_Args...), Args&&... args) ->
  __bind<R, R (*)(F_Args...), __VTT(Args, args)> {
    using namespace thrust;
    typedef R (*F)(F_Args...);

    return __bind<R, F, __VTT(Args, args)>
      (std::forward<F>(f), make_tuple(std::forward<Args>(args)...));
  }

  // Pointer to class member function
  template<typename C, typename R, typename... F_Args, typename... Args>
  __host__ __device__
  auto bind(R (C::*f)(F_Args...), Args&&... args) ->
  __bind<R, R (C::*)(F_Args...), __VTT(Args, args)> {
    using namespace thrust;
    typedef R (C::*F)(F_Args...);

    return __bind<R, F, __VTT(Args, args)>
      (std::forward<F>(f), make_tuple(std::forward<Args>(args)...));
  }

  template<typename F, typename T>
  __host__ __device__
  __bind<typename F::result_type, F, T>
  bind_tuple(F&& f, T args) {
    return __bind<typename F::result_type, F, T>
      (std::forward<F>(f), args);
  }

  template<typename R, typename F, typename... Args>
  __host__ __device__
  auto bind(F&& f, Args&&... args) ->
  __bind<R, F, __VTT(Args, args)> {
    return __bind<R, F, __VTT(Args, args)>
      (std::forward<F>(f), thrust::make_tuple(std::forward<Args>(args)...));
  }

  template<typename F, typename... Args>
  __host__ __device__
  auto bind(F&& f, Args&&... args) ->
  __bind<typename F::result_type, F, __VTT(Args, args)> {
    return __bind<typename F::result_type, F, __VTT(Args, args)>
      (std::forward<F>(f), thrust::make_tuple(std::forward<Args>(args)...));
  }

} // namespace cb
