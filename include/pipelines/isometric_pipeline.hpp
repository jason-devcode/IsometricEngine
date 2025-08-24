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

    screenSpace.setWidth(width).setHeight(height);

    return *this;
  }

  IsometricDrawerPipeline &setGraphics(Graphics *graphics) {
    shape_drawer.setGraphics(graphics);
    return *this;
  }

  void drawNormalizedLine(double x1, double y1, double x2, double y2,
                          uint32_t color) {
    Vec2f A = axisX * x1 + axisY * y1;
    Vec2f B = axisX * x2 + axisY * y2;

    int screenX1 = screenSpace.normXtoScreen(A.m_x);
    int screenY1 = screenSpace.normYtoScreen(A.m_y);
    int screenX2 = screenSpace.normXtoScreen(B.m_x);
    int screenY2 = screenSpace.normYtoScreen(B.m_y);

    shape_drawer.drawNormalizedLine(screenX1, screenY1, screenX2, screenY2,
                                    color);
  }

private:
  CoordinateSpace isometricSpace;
  ScreenSpace screenSpace;
  DrawShapes shape_drawer;
  Vec2f axisX, axisY;
};

#endif // ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP
