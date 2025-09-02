#ifndef CORE_ENGINE_INSTANCE_HPP
#define CORE_ENGINE_INSTANCE_HPP

/** for std::shared_ptr */
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <memory>

/** for uint32_t */
#include <cstdint>

/** for std::string */
#include <string>

/** for SDL Event utils */
#include <SDL2/SDL.h>

/** for EngineWindow */
#include "./engine_window.hpp"

/** for IGame */
#include "../api/igame.hpp"

/** for graphics */
#include "./graphics.hpp"

/** for TextDrawer */
#include "../utils/Text.hpp"

/** for input system */
#include "./input_system.hpp"
#include "keyboard_manager.hpp"

/** for engine_timer */
#include "./timers.hpp"

using std::uint32_t;

double deltatime = 0.0;

class EngineProps {
  public:
    EngineProps() {}
    EngineProps& setTitle( std::string title ) {
      m_title = title;
      return *this;
    }
    
    EngineProps& setScreenWidth( uint32_t width_px ) {
      m_screenWidth_px = width_px;
      return *this;
    }

    EngineProps& setScreenHeight( uint32_t height_px ) {
      m_screenHeight_px = height_px;
      return *this;
    }

    EngineProps& setScreenColor( uint32_t colorRGB888 ) {
      m_screenColor = colorRGB888;
      return *this;
    }

    EngineProps& setVSyncEnable( bool enable ) {
      m_vsyncEnable = enable;
      return *this;
    }

    EngineProps& setVSyncLimit( uint32_t limit ) {
      m_vsyncLimit = limit;
      return *this;
    }

    EngineProps& setCurrentGame( IGame* game ) {
      m_currentGame = game;
      return *this;
    }

    std::string m_title = "Default App";
    
    uint32_t m_screenWidth_px = 256;
    uint32_t m_screenHeight_px = 256;
    uint32_t m_screenColor = 0xFF000000;

    uint32_t m_vsyncLimit = 30;
    bool m_vsyncEnable = true;
    
    IGame* m_currentGame = nullptr;
};

class EngineInstance {
  public:
    EngineInstance() {
      std::cout << "EngineInstance::EngineInstance()\n";
    }
    ~EngineInstance() {
      std::cout << "EngineInstance::~EngineInstance()\n";
    }
    
    // stage 1 - create engine instance resources
    EngineInstance& onCreate() {
      window
        .setTitle( props.m_title )
        .setWidth( props.m_screenWidth_px )
        .setHeight( props.m_screenHeight_px )
        .setClearColor( props.m_screenColor )
        .create();
      return *this;
    }


    // stage 2 - initialize game engine resources
    EngineInstance& onInitialize() {
      std::cout << "EngineInstance::onInitialize - Initializing game engine resources\n";
      std::cout << std::hex;
      std::cout << "Window: 0x" << window.m_window << "\n";
      std::cout << std::nouppercase;
      std::cout << "Surface: 0x" << window.m_surface << "\n";
      std::cout << std::dec << std::nouppercase;

      if( 
          window.m_window != nullptr && 
          window.m_surface != nullptr 
      ) {
        graphics.setPixels( 
            static_cast<uint32_t*>(window.m_surface->pixels)
        );
        const auto graphWidth = window.m_surface->pitch / 4; 
        const auto graphHeight = props.m_screenHeight_px; 
        graphics.setWidth(graphWidth > 0 ? graphWidth : 1);
        graphics.setHeight(graphHeight > 0 ? graphHeight : 1);
        tout.setGraphics(&graphics);
      }
      return *this;
    }
    
    // En este punto todas las propiedades del motor ya debieron haber sido
    // inicializadas
    EngineInstance& start() {
      SDL_Event event;
      bool quit = false;
      
      std::cout << "EngineInstance::start()" << "\n";

      while( !quit ) {
        // process all events
        while( SDL_PollEvent(&event) ) {
          if( event.type == SDL_QUIT ) quit = true;
          const auto keyCode = event.key.keysym.sym;

          switch (event.type) {
            case SDL_KEYDOWN:
              input.keyboard.setKeyState( keyCode, true );
              break;
            case SDL_KEYUP:
              input.keyboard.setKeyState( keyCode, false );
              break;
          }
        }
        
        input.keyboard.triggerPressedKeyListeners();

        // Update mouse state
        SDL_GetMouseState(&input.mouse.cursorX, &input.mouse.cursorY);
        
        engine_timer.startFrame();
        window.clear();

        if( props.m_currentGame )
          props.m_currentGame->loop();

        window.render();
        engine_timer.calculateDeltatime();
      }
      if( props.m_currentGame )
        props.m_currentGame->onRelease();

     return *this;
   }

    EngineWindow window;
    InputSystem input;
    EngineProps props;
    Graphics graphics;
};



#endif // CORE_ENGINE_INSTANCE_HPP
