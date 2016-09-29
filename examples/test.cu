#include <iostream>

#include <thrust/device_vector.h>
#include <bind/bind.h>

#define N 100

template<typename T>
struct op_sum {
  typedef T result_type;

  __host__ __device__
  T operator()(T x, T y)
  { return x + y; }
};

__host__ __device__
int subtract(int x, int y) { return x - y; }

template<typename F, typename T>
__global__ void entry_point(F f, T t) {
  using namespace cb::placeholders;

  auto thread_id = threadIdx.x;
  auto sub1 = cb::bind(subtract, _1, 1);
  auto plus1 = cb::bind(op_sum<int>{}, _1, 1);

  t[thread_id] = plus1(sub1(f(t[thread_id])));
}

int main()
{
  using namespace cb::placeholders;

  auto mul = [] __host__ __device__ (int x, int y) { return x * y; };
  auto mul2 = cb::bind<int>(mul, _1, 2);

  thrust::device_vector<int> vec(N, 1);
  entry_point<<<1,N>>>(mul2, &vec[0]);

  thrust::host_vector<int> h_vec(vec);
  assert(h_vec[0] == 2 && h_vec[N-1] == 2);

  return 0;
}
