#pragma once

#include <type_traits>
#include <functional>
#include <thrust/tuple.h>
#include <thrust/detail/type_traits.h>

#include "mpl.h"
#include "placeholders.h"

namespace cb {

  template<typename F, typename... Args>
  struct __bind {

    __host__ __device__
    __bind(F&& f, Args&&... args) :
      m_fn(f), m_args(args...) {}

    template<typename... _Args>
    __host__ __device__ typename F::result_type
    operator()(_Args... args) {
      //return mpl::apply_from_tuple(m_fn, __replace_placeholders(args...));
    }

  private:
    /*template<typename... _Args>
    __host__ __device__
    auto __replace_placeholders(_Args... args) {
    }*/

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
