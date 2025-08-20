#include "../include/api/api.hpp"
#include "../include/utils/geometry.hpp"

class IsometricGame : public CGame {
  public:
    IsometricGame() {}
    ~IsometricGame() {}

    IsometricGame& onCreate() {
      engine.props
        .setTitle("Isometric Engine V0.0.1a")
        .setScreenWidth(512)
        .setScreenHeight(512)
        .setScreenColor(0xFF000000)
        .setCurrentGame(this);
      return *this;
    }

    IsometricGame& onInitialize() {
      shape_drawer.setGraphics( &engine.graphics );
      return *this;
    }

    IsometricGame& loop() {
      shape_drawer.drawCircle( 256, 256, 80, 14, 0xFF0000FF );
      //shape_drawer.drawFillCircle( 256, 256, 50, 0xFFFF0000 );
      
      shape_drawer.drawVerticalLine( 256, 0, 512, 0xFF00FF00 );
      shape_drawer.drawHorizontalLine( 256, 0, 512, 0xFF00FF00 );
      return *this;
    }

    IsometricGame& onRelease() {

      return *this;
    }

  private:
    DrawShapes shape_drawer;
};

/**
 * Game Factory used in app initialization.
 */
IsometricGame game_factory() { return IsometricGame(); }

int main( int argc, char** argv ) {
  try {
    game_factory()
      .buildEngineInstance()
      .onCreate()
      .onCreateWindow()
      .onInitialize()
      .onStart();

    return 0;
  }
  catch ( int errorCode ) {
    std::cerr << "ERROR: Code " << errorCode << "\n";
    return -1;
  }
  catch (const std::exception& error) {
    return -1;
  }
  return 0;
}

