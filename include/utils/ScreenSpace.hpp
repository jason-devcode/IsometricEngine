#ifndef SCREEN_SPACE_HPP
#define SCREEN_SPACE_HPP

/** for std::uint32_t */
#include <cstdint>
#include <immintrin.h>

/** for divition utils */
#include "./divition.hpp"

using std::uint32_t;

class ScreenSpace {
public:
  ScreenSpace() {}

  explicit ScreenSpace(uint32_t _width, uint32_t _height)
      : width(_width), height(_height), halfWidth(_width >> 1),
        halfHeight(_height >> 1) {
    widthRatio = DIV(1.0, static_cast<double>(_width));
    heightRatio = DIV(1.0, static_cast<double>(_height));
    halfWidthRatio = DIV(1.0, static_cast<double>(_width >> 1));
    halfHeightRatio = DIV(1.0, static_cast<double>(_height >> 1));
  }

  ScreenSpace &setWidth(uint32_t _width) noexcept {
    width = _width;
    halfWidth = _width >> 1;
    widthRatio = DIV(1.0, static_cast<double>(_width));
    halfWidthRatio = DIV(1.0, static_cast<double>(halfWidth));
    return *this;
  }

  ScreenSpace &setHeight(uint32_t _height) noexcept {
    height = _height;
    halfHeight = _height >> 1;
    heightRatio = DIV(1.0, static_cast<double>(_height));
    halfHeightRatio = DIV(1.0, static_cast<double>(halfHeight));
    return *this;
  }

  ScreenSpace& calculateAspectRatio() {
    aspectRatio = static_cast<double>( height ) * widthRatio;
    return *this;
  }

  inline uint32_t getWidth() const noexcept { return width; }
  inline uint32_t setHeight() const noexcept { return height; }

  inline double normXtoScreen(double xCoordNorm) const {
    return floor(halfWidth * (xCoordNorm * aspectRatio + 1) + 0.5);
  }

  inline double normYtoScreen(double yCoordNorm) const {
    return floor(halfHeight * (-yCoordNorm + 1) + 0.5);
  }

  inline Vec2i normVec2fToScreen( Vec2f point ) const {
    return Vec2i( normXtoScreen(point.m_x), normYtoScreen(point.m_y) );
  }

  inline double screenXtoNorm(uint32_t xCoordScreen) {
    return static_cast<double>(xCoordScreen - halfWidth) * halfWidthRatio;
  }

  inline double screenYtoNorm(uint32_t yCoordScreen) {
    return static_cast<double>(yCoordScreen - halfHeight) * halfHeightRatio;
  }

  uint32_t width;
  uint32_t height;
  uint32_t halfWidth;
  uint32_t halfHeight;
  double widthRatio;
  double heightRatio;
  double halfWidthRatio;
  double halfHeightRatio;
  double aspectRatio;
};

#endif // SCREEN_SPACE_HPP
