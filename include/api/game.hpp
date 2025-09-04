#ifndef ENGINE_APP_GAME_API_H
#define ENGINE_APP_GAME_API_H

/** for std::make_shared */
#include <memory>

/** for std::uint32_t */
#include <cstdint>

/** for IGame */
#include "./igame.hpp"

/** for std::cout */
#include <iostream>

/** forward declaration of EngineInstance */
class EngineInstance;

class CGame : public IGame {
  public:
    CGame() {
      std::cout << "CGame::CGame()\n";
    }
    ~CGame() {
      std::cout << "CGame::~CGame()\n";
    }
    
    // 1. stage to build game engine instance
    virtual CGame& buildEngineInstance() { 
      std::cout << "CGame::buildEngineInstance()\n";
      engine.props.setInstanceProps(
        "default",
        256,
        256,
        0xFF000000,
        true,
        60,
        this
      );
      return *this; 
    }

    virtual CGame& onCreateWindow() {
      std::cout << "CGame::onCreateWindow()\n";
      engine
        .onCreate()
        .onInitialize();
      return *this;
    }

    // 2. stage to set game instance custom props
    virtual CGame& onCreate() { return* this; }

    // 2. stage to set game assets like players, objects, assets, etc...
    virtual CGame& onInitialize() { return *this; }

    // 3. start engine
    virtual CGame& onStart() {
      engine.start();
      return *this;
    }

    // 4. stage to release all resource
    virtual CGame& onRelease() { return *this; }

    // game logic loop
  //virtual IGame& loop() {};
    virtual CGame& loop() { return *this; }

  protected:
    EngineInstance engine;
};

#endif // ENGINE_APP_GAME_API_H
