#include <iostream>
#include <bind/bind.h>

using namespace cb::placeholders;

__global__ void entry_point() {
  auto foo = [](int x, int y) { return x + y; };
  auto foo2 = cb::bind(foo, _1, 2);
}

int main()
{
  entry_point<<<1,1>>>();
  return 0;
}
