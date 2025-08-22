#ifndef VECTORIAL_CALCULUS_CORE_HPP
#define VECTORIAL_CALCULUS_CORE_HPP

/** for std::ostream */
#include <iostream>

/** for std::sqrt*/
#include <cmath>

/** for DBL_MAX */
#include <cfloat>

/** for divition utils */
#include "../utils/divition.hpp"

/*
 * Clase para realizar calculos con vectores 2D
 */
#define DEFAULT_COORD_VALUE REAL_ZERO

#define MIN_VALUE REAL_ZERO
#define MAX_VALUE DBL_MAX


class CVector2D {
  public:
    CVector2D() : m_x(0.0), m_y(0.0) {}
    CVector2D( double x, double y ) : m_x(x), m_y(y) {}

    ~CVector2D() { m_x = m_y = DEFAULT_COORD_VALUE; }

    CVector2D clone() noexcept { return CVector2D( m_x, m_y ); }
    
    CVector2D& add( const CVector2D& input ) noexcept { m_x += input.m_x; m_y += input.m_y; return *this; }
    CVector2D& sub( const CVector2D& input ) noexcept { m_x -= input.m_x; m_y -= input.m_y; return *this; }

    // multiply by scalar
    CVector2D& mul( double scalar ) noexcept { m_x *= scalar; m_y *= scalar; return *this; }
    // multiply by vector
    CVector2D& mul( const CVector2D& input ) noexcept { m_x *= input.m_x; m_y *= input.m_y; return *this; }
    
    // divide by scalar
    CVector2D& div( double scalar ) noexcept {
      double correctScalar = CORRECT_DENOMINATOR(scalar);
      m_x /= correctScalar; m_y /= correctScalar;
      return *this;
    }

    // divide by vector
    CVector2D& div( const CVector2D& input ) noexcept {
      // Protected to avoid real zero divition
      m_x /= CORRECT_DENOMINATOR( input.m_x );
      m_y /= CORRECT_DENOMINATOR( input.m_y );
      return *this;
    }

    CVector2D normalize() noexcept {
      double mod = module();

      mod = 1.0 / ((mod < LOGICAL_ZERO) ? LOGICAL_ZERO : mod);
      double normX = m_x * mod;
      double normY = m_y * mod;

      return CVector2D( normX, normY );
    }

    double module() noexcept {
      return std::sqrt( m_x * m_x + m_y * m_y );
    }

    double dot( CVector2D& input ) noexcept {
      CVector2D vecA = this->normalize();
      CVector2D vecB = input.normalize();
      return vecA.m_x * vecB.m_x + vecA.m_y * vecB.m_y;
    }

    // Overload operators
    // Arithmetic operations with other vector
    CVector2D operator+( const CVector2D& other ) const noexcept { return CVector2D( m_x + other.m_x, m_y + other.m_y ); }
    CVector2D operator-( const CVector2D& other ) const noexcept { return CVector2D( m_x - other.m_x, m_y - other.m_y ); }
    CVector2D operator*( const CVector2D& other ) const noexcept { return CVector2D( m_x * other.m_x, m_y * other.m_y ); }
    CVector2D operator/( const CVector2D& other ) const noexcept {
      return CVector2D(
        m_x / CORRECT_DENOMINATOR( other.m_x ),
        m_y / CORRECT_DENOMINATOR( other.m_y )
      );
    }

    // Arithmetic operations with scalars
    CVector2D operator+( double scalar ) noexcept { return CVector2D( m_x + scalar, m_y + scalar ); }
    CVector2D operator-( double scalar ) noexcept { return CVector2D( m_x - scalar, m_y - scalar ); }
    
    CVector2D operator*( double scalar ) noexcept { return CVector2D( m_x * scalar, m_y * scalar ); }

    CVector2D operator/( double scalar ) noexcept {
      double correctScalar = CORRECT_DENOMINATOR(scalar);
      return CVector2D( m_x / correctScalar, m_y / correctScalar );
    }

    // Assignation operators
    CVector2D& operator+=( const CVector2D& other ) noexcept {  m_x += other.m_x; m_y += other.m_y; return *this; }
    CVector2D& operator-=( const CVector2D& other ) noexcept {  m_x -= other.m_x; m_y -= other.m_y; return *this; }
    CVector2D& operator*=( const CVector2D& other ) noexcept {  m_x *= other.m_x; m_y += other.m_y; return *this; }
    CVector2D& operator/=( const CVector2D& other ) noexcept {
      m_x /= CORRECT_DENOMINATOR( other.m_x );
      m_y /= CORRECT_DENOMINATOR( other.m_y );
      return *this;
    }

    // To print vector
    friend std::ostream& operator<<( std::ostream& out, const CVector2D& input ) {
      return out << "( x: " << input.m_x << ", y: " << input.m_y << " )";
    }

    double m_x = DEFAULT_COORD_VALUE;
    double m_y = DEFAULT_COORD_VALUE;

  // Vectorial 2D Calculus
  public:
    static CVector2D calculateDirection( double angleInRadians ) {
      return CVector2D(
          std::cos( angleInRadians ),
          std::sin( angleInRadians )
      );
    }

};

#endif // VECTORIAL_CALCULUS_CORE_HPP
