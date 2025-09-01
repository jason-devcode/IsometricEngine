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


#define ROWS_PER_CHAR 8
#define TOTAL_CHARS 256
#define MAX_FONT_DATA_SIZE (ROWS_PER_CHAR * TOTAL_CHARS)

/** for text rendering by 1-bit bitmap font images */
uint8_t font_data[ MAX_FONT_DATA_SIZE ] = {
  #include "./text/font.data"
};

#ifdef FONT_DEBUG
  #define print_setter_debug(method_name, prop_name, prop_value)\
    std::cout << method_name << "()" << "\n";\
    std::cout << "Setting " << prop_name << ": " << prop_value << "\n";
#else
  #define print_setter_debug(method_name, prop_name, prop_value)
#endif

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
    FontPropsBuilder& setEnableBg( bool enableBg ) { 
      print_setter_debug( "FontPropsBuilder::setEnableBg", "enableBg", enableBg );
      props.enableBg = enableBg; return *this; 
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
        .setEnableBg(false)
        .setBackgroundColor(0x00000000)
        .setForegroundColor(0xFFFFFF00)
        .setHorizontalGap(4)
        .setVerticalGap(4)
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
      //std::cout << buffer;

      // iterate over each buffer char and render it
      // get buffer write size
      //size_t size = 
      
      auto current_char = buffer;
      auto end = fb.end();

      int baseX = props.origin_xCoord, baseY = props.origin_yCoord;
    
      if(props.enableBg) {
        int newLinesCount = fb.getNewLineCount();
        int maxLineLen = fb.getMaxLineLen() + 1;
        int paddingX = 24;
        int paddingY = 24;
        int topX = props.offset_xCoord + baseX - (paddingX >> 1) - props.horizontalGap;
        int topY = props.offset_yCoord + baseY - (paddingY >> 1) - props.verticalGap;
        int bottomX = props.offset_xCoord + ( (props.fontWidth + props.horizontalGap) * maxLineLen ) + (paddingX >> 1);
        int bottomY = props.offset_yCoord + ( (props.fontHeight + props.verticalGap) * newLinesCount ) + (paddingY >> 1);
        
        for( int Y = topY; Y < bottomY; ++Y ) {
          for( int X = topX; X < bottomX; ++X ) {
            graphics->putPixel(X,Y,props.backgroundColor);
          }
        }
      }

      // render each buffer char
      while( current_char < end ) {
        char ch = *current_char;
        
        // check if is a new line
        if( ch == '\n' ) {
          // increase baseY by font height and vertical gap
          baseY += props.fontHeight + props.verticalGap;
          // reset base x to origin
          baseX = props.origin_xCoord;
          ++current_char;
          continue;
        }

        // solve base char bitmap
        uint8_t *base_char_bitmap = &font_data[ static_cast<uint32_t>( ch ) * ROWS_PER_CHAR ];
        
        // render char bitmap
        for( int Y = 0; Y < props.fontHeight; ++Y ) {
          uint8_t rowIndex = (Y * 8) / props.fontHeight;
          uint32_t row = (base_char_bitmap[ rowIndex ] << 0);

          for( int X = 0; X < props.fontWidth; ++X ) {
            uint8_t colIndex = (X * 8) / props.fontWidth;
            uint8_t bitColor = (row >> colIndex) & 1;
            if( bitColor ) graphics->putPixel( baseX + X + props.offset_xCoord, baseY + Y + props.offset_yCoord, props.foregroundColor );
          }
        }

        // increase base X by font width and horizontal gap
        baseX += props.fontWidth + props.horizontalGap;
        ++current_char;
      }
      fb.reset();
    };


    /**
     * Generic overloading of the << operator 
     * to insert primitive data types into the buffer.
     */
    template <class T>
    std::ostream& operator<<( T value ) {
      out << value;
      return out;
    }

  // Special trigger actions
  public:
    // Special object used to force text 
    // rendering when inserted with the << operator
    class RenderText {};
    static RenderText end;
  private:
    Graphics *graphics = nullptr;       // graphics instance to draw on screen
    char buffer[MAX_BUFFER_SIZE] = {0}; // char buffer data
    std::ostream out;                   // output stream operations applied to char buffer
    fixed_string_buffer<char> fb;       // stringbuf use by ostream like storage access
};

extern TextRenderStream tout;

// overload << operator for RenderText special case
std::ostream& operator<<( std::ostream& os, TextRenderStream::RenderText ) {
  tout.render();
  return os;
}

TextRenderStream tout;

#endif // TEXT_DRAWER_UTILS_HPP
