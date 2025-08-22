#ifndef DIVITION_UTILS_HPP
#define DIVITION_UTILS_HPP

// This never should be real 0.0 or can crash application
// because is used to simulate hipotetical divition by 0.
#define LOGICAL_ZERO 0.0000001
#define REAL_ZERO 0.0

// Use this to give a direction to zero
#define CORRECT_DENOMINATOR(value)\
  (\
   ((value) > -LOGICAL_ZERO && (value) < LOGICAL_ZERO)?\
   ((value) < REAL_ZERO ? -LOGICAL_ZERO : LOGICAL_ZERO)\
   :\
   (value)\
  )

// Use this to avoid divition by zero exception
#define DIV( dividend, divisor )\
  ((dividend) / CORRECT_DENOMINATOR(divisor))

#endif // DIVITION_UTILS_HPP

