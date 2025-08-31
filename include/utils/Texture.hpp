#ifndef TEXTURE_HPP
#define TEXTURE_HPP

/** for integer types */
#include <cstdint>

/** for std::string */
#include <string>

/** for std::fill */
#include <algorithm>

/** for floor */
#include <cmath>

/** for std::cout */
#include <iostream>

class Texture {
public:
  Texture() {}
  
  Texture( 
    uint32_t _width, 
    uint32_t _height, 
    uint32_t fillColor, 
    std::string _name = "default" 
  ) : width(_width), height(_height), name(_name) {
    std::cout << "----------------------------\n";
    std::cout << "Texture::Texture( u16, u16, u32, std::string )\n";
    std::cout << "Creating Texture:\n";
    std::cout << "Name: " << _name << "\n";
    std::cout << "Width: " << _width << "\n";
    std::cout << "Height: " << _height << "\n";
    std::cout << "Fill Color: 0x" << std::hex << std::uppercase << fillColor << "\n";
    std::cout << std::dec << std::nouppercase;
    
    totalPixels = _width * _height;
    if( width > 0 && height > 1 ) {
      raster = new uint32_t[ totalPixels ]{ fillColor };
    }

    std::cout << "Total pixels: " << totalPixels << "\n";
    std::cout << "Raster Address: 0x" << std::hex << std::uppercase << raster << "\n";
    std::cout << std::dec << std::nouppercase;
    std::cout << "----------------------------\n";
  }

  ~Texture() {
    std::cout << "----------------------------\n";
    std::cout << "Texture::~Texture()\n";
    std::cout << "Destroying Texture: \n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Width: " << width << "\n";
    std::cout << "Height: " << height << "\n";
    std::cout << "Total pixels: " << totalPixels << "\n";
    std::cout << "Raster Address: 0x" << std::hex << std::uppercase << raster << "\n";
    std::cout << std::dec << std::nouppercase;
    std::cout << "----------------------------\n";

    if( width > 0 && height > 0 && raster != nullptr) {
      totalPixels = width = height = 0;
      name = "default";
      delete[] raster;
    }
  }

  uint32_t getTexelColor( double u, double v ) {
    int texelX = floor( static_cast<double>(width - 1) * fmin(fabs(u), 1.0) + 0.5 );
    int texelY = floor( static_cast<double>(height - 1) * fmin(fabs(v), 1.0) + 0.5 );

    int texelIndex = texelY * width + texelX;

    if( texelIndex > -1 && texelIndex < totalPixels )
      return raster[ texelIndex ];
    return 0xFF000000;
  }

  static Texture* createTexture( 
    uint32_t _width, 
    uint32_t _height, 
    uint32_t fillColor, 
    std::string _name = "default"
  ) {
    return (
      new Texture( 
        _width, _height, fillColor, _name 
      )
    );
  }

  static Texture* createChessTexture( 
    uint32_t _width, 
    uint32_t _height,
    uint32_t cellsWidth, uint32_t cellsHeight,
    uint32_t cellColor1, uint32_t cellColor2, 
    std::string _name = "default"
  ) {
    Texture* chess = new Texture( _width, _height, 0xFFFFFFFF, _name );
    
    for( int Y = 0; Y < _height; ++Y ) {
      for( int X = 0; X < _width; ++X ) {
        int cellX = (X * cellsWidth) / _width;
        int cellY = (Y * cellsHeight) / _height;
        
        int texelIndex = Y * _width + X;
        chess->raster[ texelIndex ] = ((cellX ^ cellY) & 1) ? cellColor1 : cellColor2;
      }
    }
    return chess;
  }

  uint32_t* getRaster() { return raster; }

  uint32_t getWidth() { return width; }
  uint32_t getHeight() { return height; }
private:
  uint32_t *raster = nullptr;
  std::string name = "default";
  uint32_t width;
  uint32_t height;
  uint32_t totalPixels;
};


#endif // TEXTURE_HPP
