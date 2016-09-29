CUDA Bind
=========

A version of `bind` for CUDA. Works from within both host and device code, and can handle `__device__` and `__host__ __device__` lambdas.

Example code:

```c++
__device__ float mul(float x, float y) { return x * y; }

__global__ my_kernel(float *) {
  using namespace cb;
  using namespace cb::placeholders;

  auto mul2 = bind(mul, _1, 2);
  auto mul24 = bind(mul, 2, 4);

  mul2(4);
  mul24();
}
```
