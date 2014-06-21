#include <iostream>
#include <functional>

#include <thrust/device_vector.h>
#include <bind/bind.h>

using namespace cb::placeholders;

template<typename T>
struct op_sum {
  typedef T result_type;

  __host__ __device__
  T operator()(T x, T y)
  { return x + y; }
};

template<typename C>
__global__ void entry_point(C& c) {
  //auto sum = [](int x, int y) { return x + y; };
  auto foo = cb::bind(op_sum<int>(), _1, 2);
  auto x = foo(2);
  printf("%d\n", x);
}

int main()
{
  thrust::device_vector<int> d_vector(10);
  entry_point<<<1,1>>>(d_vector);

  return 0;
}
