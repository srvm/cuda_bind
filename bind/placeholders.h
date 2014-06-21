#pragma once

namespace cb {
namespace placeholders {

  template<int i>
  struct __placeholder : public thrust::detail::integral_constant<int, i> {
    __host__ __device__ __placeholder() {}

    template<typename... Args>
    __host__ __device__
    typename thrust::tuple_element<i, thrust::tuple<typename std::decay<Args>::type...>>::type
    operator()(Args&&... args) const {
      using namespace thrust;

      typedef typename tuple_element<i, tuple<typename std::decay<Args>...>>::type arg_type;
      return get<i>(make_tuple(std::forward<Args>(args)...));
    }
  };

  template<typename T>
  struct is_placeholder {
    static constexpr bool value = false;
  };

  template<int i>
  struct is_placeholder<__placeholder<i>> {
    static constexpr bool value = true;
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
