#include <iostream>
#include <functional>
#include <bind/bind.h>

using namespace cb::placeholders;

template<typename T>
struct op_sum {
  typedef T result_type;

  __host__ __device__
  T operator()(T x, T y)
  { return x + y; }
};

__global__ void entry_point() {
  //auto sum = [](int x, int y) { return x + y; };
  auto foo = cb::bind(op_sum<int>(), _1, 2);
  foo(2);
}

int main()
{
  entry_point<<<1,1>>>();
  return 0;
}
