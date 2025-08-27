#ifndef ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP
#define ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP

#include <cmath>
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
constexpr double xCells = 15.0;
constexpr double yCells = 15.0;
constexpr double xAxisScale = 1 / xCells;
constexpr double yAxisScale = 1 / yCells;

#define FRONT_FACE (1 << 0)
#define LEFT_FACE (1 << 1)
#define TOP_FACE (1 << 2)

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

  void drawFace( Vec2i &A, Vec2i& B, Vec2i& C, Vec2i& D, uint32_t colorFront, uint32_t colorLeft, uint32_t colorTop, uint32_t visibilityMask ) {
    if( visibilityMask == 0 ) return;

    double dx = B.m_x - A.m_x;
    double dy = B.m_y - A.m_y;

    double xVariation = dx / (dy != 0.0 ? dy : 1 );
    double yVariation = 1;
    
    // top face
    double x1 = B.m_x; double y1 = B.m_y;
    double x2 = B.m_x; double y2 = B.m_y; 
    double x3 = A.m_x; double y3 = A.m_y;
    double x4 = D.m_x;

    // front face
    double y4 = D.m_y;
    
    int steps = fabs(dy);

    for( int linePoint = 0; linePoint <= steps; ++linePoint ) {
      // Draw top face
      if( visibilityMask & TOP_FACE ) {
        shape_drawer.drawHorizontalLine(y1+1, x1-1, x2-1, colorTop);
        shape_drawer.drawHorizontalLine(y3-1, x3-1, x4-1, colorTop);
      }
      
      // Draw front face
      if( visibilityMask & FRONT_FACE ) {
        shape_drawer.drawHorizontalLine(y4+1, x4+1, D.m_x-1, colorFront);
        shape_drawer.drawHorizontalLine(C.m_y + linePoint-1, C.m_x+1, D.m_x-1, colorFront);
        shape_drawer.drawHorizontalLine(C.m_y + linePoint + steps-1, C.m_x+1, x4-1, colorFront);
      }

      // Draw left face
      if( visibilityMask & LEFT_FACE ) {
        shape_drawer.drawHorizontalLine(y4+1, A.m_x+1, x3-1, colorLeft);
        shape_drawer.drawHorizontalLine(C.m_y + linePoint-1, A.m_x+1, C.m_x-1, colorLeft);
        shape_drawer.drawHorizontalLine(C.m_y + linePoint + steps-1, x3+1, C.m_x-1, colorLeft);
      }

      x1 -= xVariation; y1 += yVariation;
      x2 += xVariation;
      x3 -= xVariation; x4 += xVariation; y3 += yVariation;
      y4 += yVariation;
    }
  }

  void drawFillCube( 
    double x, double y, double z, 
    uint32_t frontColor, uint32_t leftColor, uint32_t topColor, 
    uint32_t visibilityMask
  ) {
    static Vec2f cubeVertices[] = {
      // top
      {0.0, 1.0},  // A
      {1.0, 1.0},  // B
      {0.0, 0.0},  // C
      {1.0, 0.0},  // D
      {-1.0,-1.0}, // F
    };

    Vec2i isometricCubeVertex[5] = {
      screenSpace.normVec2fToScreen((axisX * (cubeVertices[0].m_x + x + z) + axisY * (cubeVertices[0].m_y + y + z)) + cameraScroll),
      screenSpace.normVec2fToScreen((axisX * (cubeVertices[1].m_x + x + z) + axisY * (cubeVertices[1].m_y + y + z)) + cameraScroll),
      screenSpace.normVec2fToScreen((axisX * (cubeVertices[2].m_x + x + z) + axisY * (cubeVertices[2].m_y + y + z)) + cameraScroll),
      screenSpace.normVec2fToScreen((axisX * (cubeVertices[3].m_x + x + z) + axisY * (cubeVertices[3].m_y + y + z)) + cameraScroll),
      screenSpace.normVec2fToScreen((axisX * (cubeVertices[4].m_x + x + z) + axisY * (cubeVertices[4].m_y + y + z)) + cameraScroll)
    };
  
    static int cubeFaces[1][4] = {
      // top face
      { 0, 1, 2, 3 },
    };

    auto &topFace = cubeFaces[ 0 ];

    Vec2i& topA = isometricCubeVertex[ topFace[ 0 ] ];
    Vec2i& topB = isometricCubeVertex[ topFace[ 1 ] ];
    Vec2i& topC = isometricCubeVertex[ topFace[ 2 ] ];
    Vec2i& topD = isometricCubeVertex[ topFace[ 3 ] ];
    
    drawFace(topA, topB, topC, topD, frontColor, leftColor, topColor, visibilityMask);
  }

  Vec2i mouseCoordsToNormMap( double x, double y ) { 
    double normX = (screenSpace.halfWidthRatio * (x - screenSpace.halfWidth) / screenSpace.aspectRatio) - cameraScroll.m_x;
    double normY = (-screenSpace.halfHeightRatio * (y - screenSpace.halfHeight)) - cameraScroll.m_y;
    
    double h = normX * normX + normY * normY;
    
    double realX = isometricSpace.xAxis.m_x * normX + isometricSpace.xAxis.m_y * normY;
    double realY = isometricSpace.yAxis.m_x * normX + isometricSpace.yAxis.m_y * normY;

    double _x = sqrt( h - realY*realY );
    double _y = sqrt( h - realX*realX );

    double deltaX = _y * cos(isometricYAxisInclination - isometricXAxisInclination + 5.0 * RADIAN);
    double deltaY = _x * cos(isometricYAxisInclination - isometricXAxisInclination + 5.0 * RADIAN);
    
    realX = floor((realX - deltaX) * xCells);
    realY = floor((realY - deltaY) * yCells);

    //std::cout << "Mouse XY: " << realX << ", " << realY << "\n";

    return Vec2i( realX, realY );
  }

private:
  CoordinateSpace isometricSpace;
  ScreenSpace screenSpace;
  DrawShapes shape_drawer;
  Vec2f axisX, axisY;
  Vec2f cameraScroll;
};

#endif // ISOMETRIC_SHAPE_DRAWER_PIPELINE_HPP
