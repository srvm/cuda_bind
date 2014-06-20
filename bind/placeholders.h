#pragma once

namespace cb {
namespace placeholders {

  template<int i>
  struct __placeholder : public thrust::detail::integral_constant<int, i> {
    __host__ __device__ __placeholder() {}

    template<typename... Args>
    __host__ __device__
    typename thrust::tuple_element<i, thrust::tuple<Args...>>::type
    operator()(Args&&... args) {
      return thrust::get<i>(thrust::make_tuple(args...));
    }
  };

#ifdef __CUDA_ARCH__
  static const __device__ __placeholder<1> _1;
  static const __device__ __placeholder<2> _2;
  static const __device__ __placeholder<3> _3;
  static const __device__ __placeholder<4> _4;
  static const __device__ __placeholder<5> _5;
  static const __device__ __placeholder<6> _6;
  static const __device__ __placeholder<7> _7;
  static const __device__ __placeholder<8> _8;
  static const __device__ __placeholder<9> _9;
#else
  static const  __placeholder<1> _1;
  static const  __placeholder<2> _2;
  static const  __placeholder<3> _3;
  static const  __placeholder<4> _4;
  static const  __placeholder<5> _5;
  static const  __placeholder<6> _6;
  static const  __placeholder<7> _7;
  static const  __placeholder<8> _8;
  static const  __placeholder<9> _9;
#endif
} // namespace placeholders
} // namespace cb