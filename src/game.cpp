#include "../include/api/api.hpp"
#include "../include/utils/geometry.hpp"
#include "../include/core/vectorial.hpp"
#include "../include/utils/CoordinateSpace.hpp"
#include "../include/utils/ScreenSpace.hpp"
#include "../include/utils/trigonometry.hpp"

#include <cstdint>

using std::uint32_t;

class IsometricGame : public CGame {
  public:
    IsometricGame() {}
    ~IsometricGame() {}

    IsometricGame& onCreate() {
      engine.props
        .setTitle("Isometric Engine V0.0.1a")
        .setScreenWidth(width)
        .setScreenHeight(height)
        .setScreenColor(0xFF000000)
        .setCurrentGame(this);

      screen
        .setWidth( width )
        .setHeight( height );
      return *this;
    }

    IsometricGame& onInitialize() {
      shape_drawer.setGraphics( &engine.graphics );
      
      CVector2D c1 = (CVector2D){ 0, 0 };
      return *this;
    }
    
    double angle = 0.0;

    IsometricGame& loop() {
      //shape_drawer.drawCircle( 256, 256, 80, 14, 0xFF0000FF );
      //shape_drawer.drawFillCircle( 256, 256, 50, 0xFFFF0000 );
      
      shape_drawer.drawVerticalLine( width >> 1, 0, height, 0xFF00FF00 );
      shape_drawer.drawHorizontalLine( height >> 1, 0, width, 0xFF00FF00 );

      double x1 = std::cos( angle ) / 2, y1 = sin(angle) / 2;
      
      uint32_t xScreen, yScreen;
      xScreen = screen.normXtoScreen( x1 ), yScreen = screen.normYtoScreen( y1 );
      shape_drawer.drawFillCircle( xScreen, yScreen, 30, 0xFF00AA00 );
   
      angle += RADIAN * 0.1;
      return *this;
    }

    IsometricGame& onRelease() { return *this; }
  private:
    int width = 512 << 1;
    int height = 512 << 1;
    DrawShapes shape_drawer;
    ScreenSpace screen;
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

