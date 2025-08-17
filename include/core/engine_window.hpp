#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

// Engine Window Implementation in SDL

/** for SDL_Window and window utils */
#include <SDL2/SDL.h>

/** for std::uint32_t */
#include <cstdint>

/** for std::string */

class EngineWindow {
  public:
    EngineWindow() {

    }

    ~EngineWindow() {

    }

    EngineWindow& setTitle( std::string title ) {
      m_title = title;
    }

    EngineWindow& setWidth( std::uint32_t width ) {
      m_width = width;
    }

    EngineWindow& setHeight( std::uint32_t height ) {
      m_height = height;
    }

    EngineWindow& create() {}
  
    SDL_Window *window = nullptr;

    std::string m_title; // window title
    std::uint32_t m_width; // screen width in pixels
    std::uint32_t m_height; // screen height in pixels
};


#endif // ENGINE_WINDOW_HPP
