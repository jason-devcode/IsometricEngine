#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

/** for EngineWindow */
#include "./engine_window.hpp"

/** for std::shared_ptr */
#include <memory>

/** for uint32_t */
#include <cstdint>

/** for SDL2 surface utils */
#include <SDL2/SDL.h>

/** for I/O operators */
#include <iostream>

using std::uint32_t;

class Graphics {
  public:
    Graphics() {}
    ~Graphics() {}

    Graphics& setPixels( uint32_t* _pixels ) { 
      this->pixels = _pixels;
      return *this;
    }
    Graphics& setWidth( uint32_t _width ) { width = _width; return *this; }
    Graphics& setHeight( uint32_t _height ) { height = _height; return *this; }

    void putPixel( int32_t xCoord, int32_t yCoord, uint32_t color ) {
      // Check if pixels is initalized or coords are out of bounds
      //std::cout << "X: " << xCoord << ", Y: " << yCoord << " - 0x" << std::hex << std::uppercase << color << "\n" << std::dec;
      //std::cout << std::hex << pixels << "\n";
      //std::cout << "Graphics::putPixel() -" << pixels << "\n";
      if( 
          pixels == nullptr || 
          xCoord > width - 1 || 
          0 > xCoord || 
          yCoord > height - 1 ||
          0 > yCoord
      )
        return;
      pixels[ yCoord * width + xCoord ] = color;
    }

    uint32_t* pixels = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
};

#endif
