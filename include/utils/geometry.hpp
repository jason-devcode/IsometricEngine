#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

/** for uint32_t */
#include <cstdint>

/** for sqrt */
#include <cmath>

/** for Graphics class */
#include "../core/graphics.hpp"

#define CAN_DRAW(extra_condition)                                              \
  if (graphics == nullptr extra_condition)                                     \
    return;

using std::uint32_t;

class DrawShapes {
public:
  void drawFillCircle(int cx, int cy, int radius, uint32_t color) {
    CAN_DRAW();
    for (int Y = -radius; Y <= radius; ++Y) {
      for (int X = -radius; X <= radius; ++X) {
        if (X * X + Y * Y < radius * radius)
          graphics->putPixel(cx + X, cy + Y, color);
      }
    }
  }

  void drawCircle(int cx, int cy, int radius, int weight, uint32_t color) {
    CAN_DRAW();
    int innerRadius = radius - weight / 2;
    int outRadius = radius + weight / 2;

    // squares
    int innerRadiusSqr = innerRadius * innerRadius;
    int outRadiusSqr = outRadius * outRadius;

    for (int Y = -outRadius; Y <= outRadius; ++Y) {
      for (int X = -outRadius; X <= outRadius; ++X) {
        int hipotenuse = X * X + Y * Y;
        if (hipotenuse > innerRadiusSqr && hipotenuse < outRadiusSqr)
          graphics->putPixel(cx + X, cy + Y, color);
      }
    }
  }

  void drawVerticalLine(int x, int y1, int y2, uint32_t color) {
    // if x is out of horizontal bounds then don't draw nothing
    CAN_DRAW(|| x < 0 || x > graphics->width);

    // swap y1 and y2 if y1 is greater than y2
    if (y1 > y2) {
      y1 ^= y2;
      y2 ^= y1;
      y1 ^= y2;
    }

    for (; y1 <= y2; ++y1) {
      graphics->putPixel(x, y1, color);
    }
  }

  void drawHorizontalLine(int y, int x1, int x2, uint32_t color) {
    // if y is out of vertical bounds then don't draw nothing
    CAN_DRAW(|| y < 0 || y > graphics->height);

    // swap x1 and x2 if x1 is greater than x2
    if (x1 > x2) {
      x1 ^= x2;
      x2 ^= x1;
      x1 ^= x2;
    }

    for (; x1 <= x2; ++x1) {
      graphics->putPixel(x1, y, color);
    }
  }

  void drawNormalizedLine(int x1, int y1, int x2, int y2, uint32_t color) {
    // Check if line is visible at screen
    CAN_DRAW(|| (y1 < 0 && y2 < 0) ||
             (y1 > graphics->height && y2 > graphics->height) ||
             (x1 < 0 && x2 < 0) ||
             (x1 > graphics->width && x2 > graphics->width));
    //
    int lineLength = sqrt(x1 * x2 + y1 * y2) + 1;

    double recLenght =
        1.0 / static_cast<double>(lineLength > 0 ? lineLength : 1);

    double normX = static_cast<double>(x2 - x1) * recLenght;
    double normY = static_cast<double>(y2 - y1) * recLenght;

    double xPixel = x1;
    double yPixel = y1;

    for (int lineIterator = 0; lineIterator < lineLength; ++lineIterator) {
      graphics->putPixel(static_cast<int>(xPixel), static_cast<int>(yPixel),
                         color);
      xPixel += normX;
      yPixel += normY;
    }
  }

  DrawShapes &setGraphics(Graphics *_graphics) {
    graphics = _graphics;
    return *this;
  }

private:
  Graphics *graphics = nullptr;
};

#endif // GEOMETRY_HPP
