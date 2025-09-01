#ifndef TEXT_DRAWER_UTILS_HPP
#define TEXT_DRAWER_UTILS_HPP

/** for std::string */
#include <string>

/** for c integer types */
#include <cstdint>

/** for std::ostream */
#include <ostream>

/** for graphics */
#include "../core/graphics.hpp"

/** for fixed_string_buffer */
#include "./text/fixed_string_buffer.hpp"

typedef struct _font_props_ {
  // All these values are in pixels unit
  uint16_t fontWidth;
  uint16_t fontHeight;
  uint32_t fontWeight;
  uint32_t foregroundColor;
  uint32_t backgroundColor;
  uint32_t horizontalGap;
  uint32_t verticalGap;

  uint32_t origin_xCoord;
  uint32_t origin_yCoord;
  uint32_t offset_xCoord;
  uint32_t offset_yCoord;
  
  bool enableBg; // Enable background drawing

} FontProps;

#ifdef FONT_DEBUG
  #define print_setter_debug(method_name, prop_name, prop_value)\
    std::cout << method_name << "()" << "\n";\
    std::cout << "Setting " << prop_name << ": " << prop_value << "\n";
#else
  #define print_setter_debug(method_name, prop_name, prop_value)
#endif

class FontPropsBuilder {
  public:
    FontPropsBuilder& setFontWidth( uint32_t width ) {
      print_setter_debug( "FontPropsBuilder::setFontWidth", "width", width );
      props.fontWidth = width; return *this; 
    }

    FontPropsBuilder& setFontHeight( uint32_t height ) { 
      print_setter_debug( "FontPropsBuilder::setFontHeight", "height", height );
      props.fontHeight = height; return *this; 
    }
    FontPropsBuilder& setFontWeight( uint32_t weight ) { 
      print_setter_debug( "FontPropsBuilder::setFontWeight", "weight", weight );
      props.fontWeight = weight; return *this; 
    }
    FontPropsBuilder& setBackgroundColor( uint32_t color ) { 
      print_setter_debug( "FontPropsBuilder::setBackgroundColor", "color", 
          "0x" << std::hex << std::uppercase << color << std::dec << std::nouppercase
      );
      props.backgroundColor = color; return *this; 
    }
    FontPropsBuilder& setForegroundColor( uint32_t color ) {
      print_setter_debug( "FontPropsBuilder::setForegroundColor", "color", 
          "0x" << std::hex << std::uppercase << color << std::dec << std::nouppercase
      );
      props.foregroundColor = color; return *this; 
    }
    FontPropsBuilder& setHorizontalGap( uint32_t gap ) { 
      print_setter_debug( "FontPropsBuilder::setHorizontalGap", "horizontalGap", gap );
      props.horizontalGap = gap; return *this; 
    }
    FontPropsBuilder& setVerticalGap( uint32_t gap ) { 
      print_setter_debug( "FontPropsBuilder::setVerticalGap", "verticalGap", gap );
      props.verticalGap = gap; return *this; 
    }
    FontPropsBuilder& setOrigin( uint32_t xCoord, uint32_t yCoord ) { 
      print_setter_debug( "FontPropsBuilder::setOrigin", "xCoord, yCoord", "( " << xCoord << ", " << yCoord << " )" );
      props.origin_xCoord = xCoord; props.origin_yCoord = yCoord; return *this; 
    }
    FontPropsBuilder& setOffsets( uint32_t xCoordOffset, uint32_t yCoordOffset ) { 
      print_setter_debug( "FontPropsBuilder::setOffsets", "xCoordOffset, yCoordOffset", "( " << xCoordOffset << ", " << yCoordOffset << " )" );
      props.offset_xCoord = xCoordOffset; props.offset_yCoord = yCoordOffset; return *this; 
    }
  protected:
      FontProps props;
};

#define MAX_BUFFER_SIZE 1024

class TextRenderStream : public FontPropsBuilder {
  public:
    TextRenderStream() : fb(buffer, MAX_BUFFER_SIZE, 
      [&]() {
        std::cout << "Overflow!\n";
      }), 
    out(&fb) {
      std::cout << "TextRenderStream::TextRenderStream()\n";
      std::cout << "Setting initial font props...\n";
      (*this)
        .setFontWidth(12)
        .setFontHeight(14)
        .setFontWeight(1)
        .setBackgroundColor(0x00000000)
        .setForegroundColor(0xFFFFFF00)
        .setHorizontalGap(5)
        .setVerticalGap(5)
        .setOrigin(0,0)
        .setOffsets(0,0);
    }
    ~TextRenderStream() {
      std::cout << "TextRenderStream::~TextRenderStream()\n";
    }

    TextRenderStream& setGraphics( Graphics* _graphics ) {
      print_setter_debug( "TextRenderStream::setGraphics", "graphics", 
          std::hex << std::uppercase << _graphics << std::dec << std::nouppercase
      );
      graphics = _graphics; return *this; 
    }


    void render() {
      std::cout << buffer;
      fb.reset();
    };


    // overload << operator
    template <class T>
    std::ostream& operator<<( T value ) {
      out << value;
      return out;
    }

  // Special trigger
  public:
    class RenderText {};                // render text and clean text buffer
    static RenderText end;
  private:
    Graphics *graphics = nullptr;       // graphics instance to draw on screen
    char buffer[MAX_BUFFER_SIZE] = {0}; // char buffer data
    std::ostream out;                   // output stream operations applied to char buffer
    fixed_string_buffer<char> fb;
};

TextRenderStream tout;

// overload << operator for RenderText special case
std::ostream& operator<<( std::ostream& os, TextRenderStream::RenderText ) {
  tout.render();
  return os;
}


#endif // TEXT_DRAWER_UTILS_HPP
