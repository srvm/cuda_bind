#pragma once

namespace cb {
namespace mpl {

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
  decltype(f(thrust::get<I>(t)...)) {
    return f(thrust::get<I>(t)...);
  }

  template<typename F, typename... S>
  typename std::result_of<F(S...)>::type
  __host__ __device__
  apply_from_tuple(F f, thrust::tuple<S...> s) {
    return __delayed_dispatch(f, s, typename gen<sizeof... (S)>::type());
  }

} // namespace mpl
} // namespace cb
