#include <iostream>

#include <thrust/device_vector.h>
#include <bind/bind.h>

using namespace cb::placeholders;

template<typename T>
struct op_sum {
  typedef T result_type;

  __host__ __device__
  T operator()(T x, T y)
  { return x - y; }
};

template<typename T>
struct op_subtract {
  typedef T result_type;

  __host__ __device__
  T operator()(T x, T y)
  { return x - y; }
};

__device__ int subtract(int x, int y)
{ return x - y; }

template<typename C>
__global__ void entry_point(C& c) {
  auto subtract_lambda = [](int x, int y) { return x - y; };

  //auto foo = cb::bind(op_subtract<int>(), 1, 2);
  auto foo = cb::bind(subtract, 2, _1);
  //auto foo = cb::bind(subtract_lambda, 2, 1);
  auto x = foo(1);
  printf("%d\n", x);
}

int main()
{
  thrust::device_vector<int> d_vector(10);
  entry_point<<<1,1>>>(d_vector);

  /*auto foo = cb::bind(op_subtract<int>(), 2, 1);
  auto x = foo(2, 4);
  printf("%d\n", x);*/

  return 0;
}
