#include "../include/api/api.hpp"
#include "../include/core/vectorial.hpp"
#include "../include/utils/CoordinateSpace.hpp"
#include "../include/utils/ScreenSpace.hpp"
#include "../include/utils/geometry.hpp"
#include "../include/utils/trigonometry.hpp"

#include "../include/pipelines/isometric_pipeline.hpp"

#include <cstdint>

using std::uint32_t;

class IsometricGame : public CGame {
public:
  IsometricGame() {}
  ~IsometricGame() {}

  IsometricGame &onCreate() {
    engine.props.setTitle("Isometric Engine V0.0.1a")
        .setScreenWidth(width)
        .setScreenHeight(height)
        .setScreenColor(0xFF000000)
        .setCurrentGame(this);
    return *this;
  }

  IsometricGame &onInitialize() {
    isometric_drawer.setGraphics(&engine.graphics).initalize(width, height);
    return *this;
  }
  
  double X = 0.5;
  double Y = 0.5;

  IsometricGame &loop() {

    const double axisLength = 12;
    const double offset = -4;

    isometric_drawer.drawNormalizedLine(offset, offset, offset + axisLength, offset, 0xFFFF0000);
    isometric_drawer.drawNormalizedLine(offset, offset, offset, offset + axisLength, 0xFF00FF00);
    isometric_drawer.drawNormalizedLine(offset - 1, offset - 1, offset - 1, offset + axisLength - 1, 0xFF00FF00);
    isometric_drawer.drawNormalizedLine(offset - 1, offset - 1, offset + axisLength - 1, offset - 1, 0xFFFF0000);

    for (int i = 0; i <= axisLength; ++i) {
      isometric_drawer.drawNormalizedLine(offset + i, offset, offset + i - 1, offset - 1, 0xFFFF0000);
      isometric_drawer.drawNormalizedLine(offset, offset + i, offset - 1, offset + i - 1, 0xFF00FF00);

      isometric_drawer.drawNormalizedLine(offset, offset+i, offset+axisLength, offset+i, 0xFF00FF00);
      isometric_drawer.drawNormalizedLine(offset+i, offset, offset+i, offset+axisLength, 0xFFFF0000);
    }

  
    for (int i = 0; i < axisLength; ++i) {
      isometric_drawer.drawFillCircle( offset + X, offset + 0.5 + i + fabs(sin(X * HALF_PI)), 8, 0xFF00A0FF );
    }
    X = X < axisLength ? X + 0.001 : 0.5;

    return *this;
  }

  IsometricGame &onRelease() { return *this; }

private:
  IsometricDrawerPipeline isometric_drawer;
  int width = 1280;
  int height = 720;
};

/**
 * Game Factory used in app initialization.
 */
IsometricGame game_factory() { return IsometricGame(); }

int main(int argc, char **argv) {
  try {
    game_factory()
        .buildEngineInstance()
        .onCreate()
        .onCreateWindow()
        .onInitialize()
        .onStart();

    return 0;
  } catch (int errorCode) {
    std::cerr << "ERROR: Code " << errorCode << "\n";
    return -1;
  } catch (const std::exception &error) {
    return -1;
  }
  return 0;
}
