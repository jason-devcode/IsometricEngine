#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

// Engine Window Implementation in SDL

/** for SDL_Window and window utils */
#include <SDL2/SDL.h>

/** for std::uint32_t */
#include <cstdint>

/** for std::string */
#include <string>

/** for std::except */
#include <exception>

/** for std::cerr */
#include <iostream>

/** WINDOW INITIALIZATION ERROR CODE */
#define ERROR_VIDEO_INIT_SYSTEM 100

/** WINDOW CREATION ERROR CODE */
#define ERROR_WINDOW_CREATION 101

/** WINDOW GETTING SURFACE ERROR CODE */
#define ERROR_WINDOW_GET_SURFACE 102


/** utility to initialize SDL Video System */
#define INITIALIZE_SDL_VIDEO_SYS()\
  if( !SDL_Init( SDL_INIT_VIDEO ) < 0 ) {\
    std::cerr << "ERROR (SDL Init): " << SDL_GetError() << "\n";\
    throw ERROR_VIDEO_INIT_SYSTEM;\
  }


class EngineWindow {
  public:
    EngineWindow() {

    }

    ~EngineWindow() {

    }

    EngineWindow& setTitle( std::string title ) { m_title = title; return *this; }

    EngineWindow& setWidth( std::uint32_t width ) { m_width = width; return *this; }

    EngineWindow& setHeight( std::uint32_t height ) { m_height = height; return *this; }
    
    // 1.stage initialize SDL Window system
    EngineWindow& initalize() {
      INITIALIZE_SDL_VIDEO_SYS();
      return *this;
    }

    EngineWindow& create() {
      // 1. Create Window
      SDL_Window* window = SDL_CreateWindow( 
          m_title.c_str(), 
          SDL_WINDOWPOS_UNDEFINED, 
          SDL_WINDOWPOS_UNDEFINED,
          m_width,
          m_height,
          SDL_WINDOW_SHOWN
       );
      
      // if theres some error while perform window creation
      if( window == nullptr ) {
        std::cerr << "EngineWindow::create() - Window Creation Error: " << SDL_GetError() << "\n";
        throw ERROR_WINDOW_CREATION;
      }

      // 2. Get Window Surface
      SDL_Surface *surface = SDL_GetWindowSurface( window );

      // if theres some error while get window surface
      if( surface == nullptr ) {
        std::cerr << "EngineWindow::create() - Getting Window Surface Error: " << SDL_GetError() << "\n";
        throw ERROR_WINDOW_GET_SURFACE;
      }

      // 3. fill window screen surface with white rect
      SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );

      // 4. Update window surface
      SDL_UpdateWindowSurface( window );

      // store in object state
      m_window = window;
      m_surface = surface;

      return *this;
    }
  
    SDL_Window *m_window = nullptr;
    SDL_Surface* m_surface = nullptr;

    std::string m_title; // window title
    std::uint32_t m_width; // screen width in pixels
    std::uint32_t m_height; // screen height in pixels
};


#endif // ENGINE_WINDOW_HPP
