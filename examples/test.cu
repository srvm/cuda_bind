#include <iostream>
#include <bind/bind.h>

int main()
{
  using namespace thrust::bind;
  using namespace thrust::bind::placeholders;

  std::cout << decltype(_4)::value << std::endl;

  return 0;
}
