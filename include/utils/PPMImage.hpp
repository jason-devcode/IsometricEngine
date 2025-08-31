#ifndef PPM_IMAGE_FORMAT_HPP
#define PPM_IMAGE_FORMAT_HPP

/** for std::cout */
#include <cstdint>
#include <iostream>

/** for std::string */
#include <string>

/** for std::fstream */
#include <fstream>

/** for std::stringstream */
#include <sstream>

/** for Texture */
#include "./Texture.hpp"


class PPMImage {
public:
  PPMImage() {
    std::cout << "PPMImage::PPMImage()\n";
  }
  ~PPMImage() {
    std::cout << "PPMImage::~PPMImage()\n";
  }
  
  static Texture* readFile( std::string filename ) {
    std::cout << "PPMImage::readFile() - Opening Image: " << filename << "\n";

    std::fstream ppmFile( filename, std::ios::binary | std::ios::in );

    if( !ppmFile.is_open() ) {
      std::cerr << "PPMImage::readFile() - Error: " << "Cannot load PPM file image " << filename << "\n";
      return nullptr;
    }

    std::string ppmHeader;
    std::getline( ppmFile, ppmHeader );

    if( ppmHeader != "P6" ) {
      std::cerr << "PPMImage::readFile() - Error: Invalid file format!\n";
      std::cerr << "PPMImage::readFile() - Expected P6 but get " << ppmHeader << "\n";
      return nullptr;
    }

    std::string dimensions;
    std::getline( ppmFile, dimensions );
    std::string maxBright;
    std::getline( ppmFile, maxBright );

    std::stringstream ss(dimensions);

    int width, height;
    ss >> width >> height;

    std::cout << ppmHeader << "\n";
    std::cout << "Width: " << width << " Height: " << height << "\n";

    int totalPixels = width * height;

    Texture* output = Texture::createTexture(width, height, 0xFFFFFFFF);
    uint32_t* raster = output->getRaster();

    for( int rgbIndex = 0; rgbIndex < totalPixels; ++rgbIndex ) {
      uint8_t red = 0;
      uint8_t green = 0;
      uint8_t blue = 0;
      ppmFile.read(reinterpret_cast<char*>(&red), 1);
      ppmFile.read(reinterpret_cast<char*>(&green), 1);
      ppmFile.read(reinterpret_cast<char*>(&blue), 1);
      uint32_t texelColor = 0xFF000000 | ( (red << 16) | (green << 8) | blue );
      *raster = texelColor;
      ++raster;
    }

    ppmFile.close();

    return output;
  }
};


#endif // PPM_IMAGE_FORMAT_HPP
