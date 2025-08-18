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
        .setScreenColor(0xFFFFFF)
        .setCurrentGame(
            std::make_shared<IsometricGame>(*this)
        );
      return *this;
    }

    IsometricGame& onInitialize() {

      return *this;
    }

    IsometricGame& loop() {
      static int iterationCounter = 0;
      std::cout << ++iterationCounter << "\n";
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
      .onInitialize()
      .onCreateWindow()
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

