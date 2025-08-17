#ifndef ENGINE_APP_GAME_API_H
#define ENGINE_APP_GAME_API_H

/** for std::make_shared */
#include <memory>

/** for std::uint32_t */
#include <cstdint>

/** forward declaration of EngineInstance */
class EngineInstance;

class CGame {
  public:
    CGame() {}
    ~CGame() {}
    
    // 1. stage to build game engine instance
    CGame& buildEngineInstance() { 
      engine = EngineInstance();
      engine.props 
        .setTitle("default")
        .setScreenWidth(256)
        .setScreenHeight(256)
        .setScreenColor(0xFF000000)
        .setVSyncEnable(true)
        .setVSyncLimit(60)
        .setCurrentGame(
            std::make_shared<CGame>(*this)
        );
      return *this; 
    }

    // 2. stage to set game instance custom props
    virtual CGame& onCreate() { return* this; }

    // 2. stage to set game assets like players, objects, assets, etc...
    virtual CGame& onInitialize() { return *this; }

    // 3. start engine
    CGame& onStart() {
      engine.start();
      return *this;
    }

    // 4. stage to release all resource
    virtual CGame& onRelease() { return *this; }

    // game logic loop
    virtual CGame& gameLoop() { return *this; }

  protected:
    EngineInstance engine;
};

#endif // ENGINE_APP_GAME_API_H
