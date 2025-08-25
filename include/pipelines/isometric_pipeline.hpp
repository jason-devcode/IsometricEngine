#ifndef ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP
#define ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP

#include <cstdint>
#include <iostream>
#include <sys/types.h>

using std::uint32_t;

#include "../core/graphics.hpp"
#include "../core/vectorial.hpp"
#include "../utils/CoordinateSpace.hpp"
#include "../utils/ScreenSpace.hpp"
#include "../utils/geometry.hpp"
#include "../utils/trigonometry.hpp"

constexpr double isometricXAxisInclination = degreesToRadians(30);
constexpr double isometricYAxisInclination = degreesToRadians(90 + 60);
constexpr double xAxisScale = 1 / 10.0;
constexpr double yAxisScale = 1 / 10.0;

class IsometricDrawerPipeline {
public:
  IsometricDrawerPipeline() {
    std::cout << "IsometricDrawerPipeline::IsometricDrawerPipeline()\n";
  }
  ~IsometricDrawerPipeline() {
    std::cout << "IsometricDrawerPipeline::~IsometricDrawerPipeline()\n";
  }

  IsometricDrawerPipeline &initalize(uint32_t width, uint32_t height) {
    std::cout << "IsometricDrawerPipeline::initalize()\n";

    isometricSpace.setXAxisInclination(isometricXAxisInclination)
        .setYAxisInclination(isometricYAxisInclination)
        .setXAxisScale(xAxisScale)
        .setYAxisScale(yAxisScale)
        .calculateAxisDirections();

    axisX = isometricSpace.getScaledXAxis();
    axisY = isometricSpace.getScaledYAxis();

    screenSpace
      .setWidth(width)
      .setHeight(height)
      .calculateAspectRatio();

    return *this;
  }

  IsometricDrawerPipeline &setGraphics(Graphics *graphics) {
    std::cout << "IsometricDrawerPipeline::setGraphics()\n";
    shape_drawer.setGraphics(graphics);
    return *this;
  }

  Vec2f* getCameraScroll() { return &cameraScroll; }

  void drawNormalizedLine(double x1, double y1, double x2, double y2, uint32_t color) {
    Vec2f A = (axisX * x1 + axisY * y1) + cameraScroll;
    Vec2f B = (axisX * x2 + axisY * y2) + cameraScroll;
    
    Vec2i screenPointA = screenSpace.normVec2fToScreen(A);
    Vec2i screenPointB = screenSpace.normVec2fToScreen(B);

    shape_drawer.drawLine(
      screenPointA.m_x, screenPointA.m_y, 
      screenPointB.m_x, screenPointB.m_y, 
      color
    );
  }

  void drawFillCircle( double cx, double cy, double radius, uint32_t color ) {
    Vec2f center = (axisX * cx + axisY * cy) + cameraScroll;
    Vec2i screenPoint = screenSpace.normVec2fToScreen(center);
    shape_drawer.drawFillCircle(screenPoint.m_x, screenPoint.m_y, radius, color);
  }
private:
  CoordinateSpace isometricSpace;
  ScreenSpace screenSpace;
  DrawShapes shape_drawer;
  Vec2f axisX, axisY;
  Vec2f cameraScroll;
};

#endif // ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP
