#pragma once

#include <type_traits>

namespace thrust {
namespace bind {
namespace placeholders {

  template<int i>
  struct placeholder : public std::integral_constant<int, i> {};

  extern placeholder<1> _1;
  extern placeholder<2> _2;
  extern placeholder<3> _3;
  extern placeholder<4> _4;
  extern placeholder<5> _5;
  extern placeholder<6> _6;
  extern placeholder<7> _7;
  extern placeholder<8> _8;
  extern placeholder<9> _9;
} // namespace placeholder


} // namespace bind
} // namespace thrust
