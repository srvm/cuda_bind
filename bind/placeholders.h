#pragma once

#include "mpl.h"

namespace cb {
namespace placeholders {

  template<int i>
  struct __placeholder {
    typedef void __is_placeholder;
    template<typename T>
    using result_type = typename thrust::tuple_element<i, T>::type;

    __host__ __device__ __placeholder() {}

    template<typename T>
    __host__ __device__
    typename thrust::tuple_element<i, T>::type operator()(T t) {
      static_assert(i < thrust::tuple_size<T>::value,
          "Error: Placeholder does not point to anything");
      return thrust::get<i>(t);
    }
  };

  // Allow user-defined placeholders by having
  // an '__is_placeholder' type inside.
  __CHECK_NESTED_TYPE(__is_placeholder, check_placeholder);

  template<typename T> using is_placeholder = check_placeholder<T>;

  template<typename T>
  struct has_placeholder {
    typedef typename T::head_type THT;
    typedef typename T::tail_type TTT;

    static constexpr bool value = is_placeholder<THT>::value ||
      has_placeholder<TTT>::value;
  };

  template<typename _T>
  struct has_placeholder<thrust::detail::cons<_T, thrust::null_type>> {
    static constexpr bool value = is_placeholder<_T>::value;
  };

#ifdef __CUDA_ARCH__
  static const __device__ __placeholder<0> _1;
  static const __device__ __placeholder<1> _2;
  static const __device__ __placeholder<2> _3;
  static const __device__ __placeholder<3> _4;
  static const __device__ __placeholder<4> _5;
  static const __device__ __placeholder<5> _6;
  static const __device__ __placeholder<6> _7;
  static const __device__ __placeholder<7> _8;
  static const __device__ __placeholder<8> _9;
#else
  static const  __placeholder<0> _1;
  static const  __placeholder<1> _2;
  static const  __placeholder<2> _3;
  static const  __placeholder<3> _4;
  static const  __placeholder<4> _5;
  static const  __placeholder<5> _6;
  static const  __placeholder<6> _7;
  static const  __placeholder<7> _8;
  static const  __placeholder<8> _9;
#endif
} // namespace placeholders
} // namespace cb
