#ifndef CLASS_POINT_2D_HPP
#define CLASS_POINT_2D_HPP

#include <cmath>
#include "./trigonometry.hpp"
#include "../core/vectorial.hpp"

constexpr double DEFAULT_XAXIS_INCLINATION = 0.0;
constexpr double DEFAULT_YAXIS_INCLINATION = trigonometry::HALF_PI;

class CoordinateSpace {
  public:
    explicit CoordinateSpace() {
      xAxisInclination = DEFAULT_XAXIS_INCLINATION;
      yAxisInclination = DEFAULT_YAXIS_INCLINATION;
      calculateAxisDirections();
    }
    
    ~CoordinateSpace() {}

    CoordinateSpace& setXAxisInclination( double angleInRadians ) noexcept {
      xAxisInclination = angleInRadians;
      return *this;
    }

    CoordinateSpace& setYAxisInclination( double angleInRadians ) noexcept {
      yAxisInclination = angleInRadians;
      return *this;
    }

    double getXAxisInclination() const noexcept { return xAxisInclination; }
    double getYAxisInclination() const noexcept { return yAxisInclination; }

    const CVector2D& getXAxisDirection() const noexcept { return xAxis; }
    const CVector2D& getYAxisDirection() const noexcept { return yAxis; }

    CoordinateSpace& calculateXAxisDirection() noexcept {
      xAxis = CVector2D::calculateDirection( xAxisInclination );
      return *this;
    }

    CoordinateSpace& calculateYAxisDirection() noexcept {
      yAxis = CVector2D::calculateDirection( yAxisInclination );
      return *this;
    }

    CoordinateSpace& calculateAxisDirections() noexcept {
      return calculateXAxisDirection().calculateYAxisDirection();
    }


    // unitary vectors to indicate the plane axis direction
    CVector2D yAxis; // unitary vector to indicate the direction of yAxis
    CVector2D xAxis; // unitary vector to indicate the direction of xAxis
    
    // plane axis inclinations in radians
    double yAxisInclination; // By default PI/2 radians
    double xAxisInclination; // By default 0 radians                             
};

// Overload operator *= to apply coordinate space to a vector
CVector2D& operator*=( CVector2D& vec, CoordinateSpace& cs ) {
  vec = (cs.xAxis * vec.m_x) + (cs.yAxis * vec.m_y);
  return vec;
}


#endif
