#include "../include/api/api.hpp"

class IsometricGame : public CGame {
  public:
    IsometricGame() {}
    ~IsometricGame() {}

    CGame& onCreate() {
      engine.props
        .setTitle("Isometric Engine V0.0.1a")
        .setScreenWidth(512)
        .setScreenHeight(512)
        .setScreenColor(0xFFFFFF);
      return *this;
    }

    CGame& onInitialize() {

      return *this;
    }

    CGame& loop() {

      return *this;
    }

    CGame& onRelease() {

      return *this;
    }
};

/**
 * Game Factory used in app initialization.
 */
CGame game_factory() { return IsometricGame(); }

int main( int argc, char** argv ) {
  try {
    game_factory()
      .buildEngineInstance()
      .onCreate()
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

