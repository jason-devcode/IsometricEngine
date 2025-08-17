#include <iostream>
#include "../include/core/vectorial.hpp"


int main (int argc, char *argv[]) {
  CVector2D a( 10, 0 );
  CVector2D b( 0, 10 );

  double scalar = 5.0;

  std::cout << "vec a: " << a << "\n";
  std::cout << "vec b: " << b << "\n";
  
  std::cout << "Vec a normalized: " << a.normalize() << "\n";
  std::cout << "Vec b normalized: " << b.normalize() << "\n";

  std::cout << "Dot product between a and b: " << a.dot(b) << "\n";
  std::cout << "a + b = " << a + b << "\n";
  std::cout << "a - b = " << a - b << "\n";
  std::cout << "a * b = " << a * b << "\n";
  std::cout << "a / b = " << a / b << "\n";
  std::cout << "b / a = " << b / a << "\n";

  std::cout << "a + " << scalar << " = " << a + scalar << "\n";
  std::cout << "a - " << scalar << " = " << a - scalar << "\n";
  std::cout << "a * " << scalar << " = " << a * scalar << "\n";
  std::cout << "a / " << scalar << " = " << a / scalar << "\n";


  return 0;
}

