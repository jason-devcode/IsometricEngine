#include "../include/api/api.hpp"

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
      //std::cout << engine.graphics.pixels << std::endl ;
      return *this;
    }

    IsometricGame& loop() {
      for( int Y = 0; Y < 256; Y++ ) {
        for( int X = 0; X < 256; X++ ) {
          engine.graphics.putPixel(X, Y, 0xFF0000FF);
        }
      }
      return *this;
    }

    IsometricGame& onRelease() {

      return *this;
    }
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

