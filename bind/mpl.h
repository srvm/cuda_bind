#pragma once

namespace cb {
namespace mpl {

  // apply_from_tuple code derived from this Stack Overflow thread:
  // http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

  template<int...>
  struct indices {};

  template<int N, int... S>
  struct gen : gen<N-1, N-1, S...> {};

  template<int... S>
  struct gen<0, S...> {
    typedef indices<S...> type;
  };

  template<typename F, typename T, int... I>
  __host__ __device__
  auto __delayed_dispatch(F f, T t, indices<I...>) ->
  decltype(f(thrust::get<I>(t)...))
  { return f(thrust::get<I>(t)...); }

  template<typename F, typename T>
  __host__ __device__ typename F::result_type
  apply_from_tuple(F f, T t) {
    return __delayed_dispatch(f, t,
        typename gen<thrust::tuple_size<T>::value>::type());
  }

  template<typename R, typename F, typename T>
  __host__ __device__ R
  apply_from_tuple(F f, T t) {
    return __delayed_dispatch(f, t,
        typename gen<thrust::tuple_size<T>::value>::type());
  }

#if 0
  template<typename T1, typename T2>
  struct __tuple_cat {};

  template<typename... _T1, typename... _T2>
  struct __tuple_cat<thrust::tuple<_T1...>, thrust::tuple<_T2...>> {
    typedef thrust::tuple<_T1..., _T2...> type;

    __host__ __device__
    static type apply(thrust::tuple<_T1...> t1, thrust::tuple<_T2...> t2) {
      return __delayed_apply(t1, t2,
                             typename gen<sizeof... (_T1)>::type(),
                             typename gen<sizeof... (_T2)>::type());
    }

    template<int... I1, int... I2>
    __host__ __device__
    static type __delayed_apply(thrust::tuple<_T1...> t1,
                                thrust::tuple<_T2...> t2,
                                indices<I1...>, indices<I2...>) {
      return thrust::make_tuple(thrust::get<I1>(t1)...,
                                thrust::get<I2>(t2)...);
    }
  };

  template<typename T1, typename T2>
  __host__ __device__
  typename __tuple_cat<T1, T2>::type
  tuple_cat(T1 t1, T2 t2) {
    //return __tuple_cat<T1, T2>::apply(t1, t2);
  }
#endif

} // namespace mpl
} // namespace cb
