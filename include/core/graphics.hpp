#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

/** for EngineWindow */
#include "./engine_window.hpp"

/** for std::shared_ptr */
#include <memory>

/** for std::uint32_t */
#include <cstdint>

/** for SDL2 surface utils */
#include <SDL2/SDL.h>

class Graphics {
  public:
    Graphics() {}
    ~Graphics() {}

    Graphics& setPixels( std::uint32_t* _pixels ) { pixels = _pixels; return *this; }
    Graphics& setWidth( std::uint32_t _width ) { width = _width; return *this; }
    Graphics& setHeight( std::uint32_t _height ) { height = _height; return *this; }

    void putPixel( int32_t xCoord, int32_t yCoord, uint32_t color ) {
      // Check if pixels is initalized or coords are out of bounds
      if( 
          pixels == nullptr || 
          width < xCoord || 
          0 > xCoord || 
          height < yCoord ||
          0 > yCoord
      )
        return;
      pixels[ yCoord * width + xCoord ] = color;
    }
  private:
    std::uint32_t* pixels = nullptr;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
};

#endif
