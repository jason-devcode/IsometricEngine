#ifndef TRIGONOMETRY_UTILS_HPP
#define TRIGONOMETRY_UTILS_HPP

namespace trigonometry {

  // Trigonometry constants
  constexpr double PI = 3.1415927;
  constexpr double HALF_PI = PI * 0.5;
  constexpr double TAU = PI * 2.0;

  constexpr double RADIAN = 0.01746;

  // Degrees
  constexpr double HALF_CIRCLE = 180.0;

  constexpr inline double degreesToRadians( double degrees ) noexcept {
    return RADIAN * degrees;
  }
}


#endif
