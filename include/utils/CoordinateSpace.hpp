#ifndef CLASS_POINT_2D_HPP
#define CLASS_POINT_2D_HPP

#include "../core/vectorial.hpp"
#include "./trigonometry.hpp"
#include <cmath>

constexpr double DEFAULT_XAXIS_INCLINATION = 0.0;
constexpr double DEFAULT_YAXIS_INCLINATION = HALF_PI;

class CoordinateSpace {
public:
  explicit CoordinateSpace() {
    xAxisInclination = DEFAULT_XAXIS_INCLINATION;
    yAxisInclination = DEFAULT_YAXIS_INCLINATION;
    calculateAxisDirections();
  }

  ~CoordinateSpace() {}

  CoordinateSpace &setXAxisInclination(double angleInRadians) noexcept {
    xAxisInclination = angleInRadians;
    return *this;
  }

  CoordinateSpace &setYAxisInclination(double angleInRadians) noexcept {
    yAxisInclination = angleInRadians;
    return *this;
  }

  CoordinateSpace &setXAxisScale(double scale) noexcept {
    scaleX = scale;
    return *this;
  }

  CoordinateSpace &setYAxisScale(double scale) noexcept {
    scaleY = scale;
    return *this;
  }

  double getXAxisInclination() const noexcept { return xAxisInclination; }
  double getYAxisInclination() const noexcept { return yAxisInclination; }

  const Vec2f &getXAxisDirection() const noexcept { return xAxis; }
  const Vec2f &getYAxisDirection() const noexcept { return yAxis; }

  Vec2f getScaledXAxis() noexcept { return xAxis * scaleX; }
  Vec2f getScaledYAxis() noexcept { return yAxis * scaleY; }

  CoordinateSpace &calculateXAxisDirection() noexcept {
    xAxis = Vec2f::calculateDirection(xAxisInclination);
    return *this;
  }

  CoordinateSpace &calculateYAxisDirection() noexcept {
    yAxis = Vec2f::calculateDirection(yAxisInclination);
    return *this;
  }

  CoordinateSpace &calculateAxisDirections() noexcept {
    return calculateXAxisDirection().calculateYAxisDirection();
  }

  // unitary vectors to indicate the plane axis direction
  Vec2f yAxis; // unitary vector to indicate the direction of yAxis
  Vec2f xAxis; // unitary vector to indicate the direction of xAxis

  // plane axis inclinations in radians
  double yAxisInclination; // By default PI/2 radians
  double xAxisInclination; // By default 0 radians

  // plane axis scale
  double scaleX = 1.0, scaleY = 1.0;
};

// Overload operator *= to apply coordinate space to a vector
Vec2f &operator*=(Vec2f &vec, CoordinateSpace &cs) {
  vec = (cs.xAxis * vec.m_x) + (cs.yAxis * vec.m_y);
  return vec;
}

#endif
