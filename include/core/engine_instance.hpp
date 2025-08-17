#ifndef CORE_ENGINE_INSTANCE_HPP
#define CORE_ENGINE_INSTANCE_HPP

/** for std::shared_ptr */
#include <memory>

/** for std::uint32_t */
#include <cstdint>

/** for std::string */
#include <string>

/** for SDL Event utils */
#include <SDL2/SDL.h>

/** for EngineWindow */
#include "./engine_window.hpp"


/** forward declaration of CGame */
class CGame;

class EngineProps {
  public:
    EngineProps() {}
    EngineProps& setTitle( std::string title ) {
      m_title = title;
      return *this;
    }
    
    EngineProps& setScreenWidth( std::uint32_t width_px ) {
      m_screenWidth_px = width_px;
      return *this;
    }

    EngineProps& setScreenHeight( std::uint32_t height_px ) {
      m_screenHeight_px = height_px;
      return *this;
    }

    EngineProps& setScreenColor( std::uint32_t colorRGB888 ) {
      m_screenColor = colorRGB888;
      return *this;
    }

    EngineProps& setVSyncEnable( bool enable ) {
      m_vsyncEnable = enable;
      return *this;
    }

    EngineProps& setVSyncLimit( std::uint32_t limit ) {
      m_vsyncLimit = limit;
      return *this;
    }

    EngineProps& setCurrentGame( std::shared_ptr<CGame> game ) {
      m_currentGame = std::move(game);
      return *this;
    }

    std::string m_title = "Default App";
    
    std::uint32_t m_screenWidth_px = 256;
    std::uint32_t m_screenHeight_px = 256;
    std::uint32_t m_screenColor = 0xFF000000;

    std::uint32_t m_vsyncLimit = 30;
    bool m_vsyncEnable = true;
    
    std::shared_ptr<CGame> m_currentGame = nullptr;
};

class EngineInstance {
  public:
    EngineInstance() {
      props = EngineProps();
      window = EngineWindow();
    }
    
    // 
    EngineInstance& onInitialize() {
      
      return *this;
    }
    
    // En este punto todas las propiedades del motor ya debieron haber sido
    // inicializadas
    EngineInstance& start() {
      window
        .setTitle( props.m_title )
        .setWidth( props.m_screenWidth_px )
        .setHeight( props.m_screenHeight_px )
        .create();

      SDL_Event event;
      bool quit = false;

      while( !quit ) {
        // process all events
        while( SDL_PollEvent(&event) ) {
          if( event.type == SDL_QUIT ) quit = true;
        }
      }
      return *this;
    }

    EngineWindow window;
    EngineProps props;
};



#endif // CORE_ENGINE_INSTANCE_HPP
