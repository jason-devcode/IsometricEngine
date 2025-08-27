#include "../include/api/api.hpp"
#include "../include/core/vectorial.hpp"
#include "../include/utils/CoordinateSpace.hpp"
#include "../include/utils/ScreenSpace.hpp"
#include "../include/utils/geometry.hpp"
#include "../include/utils/trigonometry.hpp"

#include "../include/pipelines/isometric_pipeline.hpp"

#include <cstdint>
#include <sys/types.h>

using std::uint32_t;

class IsometricGame : public CGame {
public:
  IsometricGame() {
    std::cout << "IsometricGame::IsometricGame()\n";
  }
  ~IsometricGame() {
    std::cout << "IsometricGame::~IsometricGame()\n";
  }

  IsometricGame &onCreate() {
    engine.props.setTitle("Isometric Engine V0.0.1a")
        .setScreenWidth(width)
        .setScreenHeight(height)
        .setScreenColor(0xFF101010)
        .setCurrentGame(this);
    return *this;
  }
  
  Vec2f *cameraScroll = nullptr;


  IsometricGame &onInitialize() {
    isometric_drawer
      .setGraphics(&engine.graphics)
      .initalize(width, height);

    auto& keyboard = engine.input.keyboard;
    
    cameraScroll = isometric_drawer.getCameraScroll();
    cameraScroll->m_y -= 1;

    constexpr double cameraStep = 0.003;

    keyboard.addKeyPressed( 'a', [&](EventData event) { cameraScroll->m_x += cameraStep; } );
    keyboard.addKeyPressed( 'd', [&](EventData event) { cameraScroll->m_x -= cameraStep; } );
    keyboard.addKeyPressed( 'w', [&](EventData event) { cameraScroll->m_y -= cameraStep; } );
    keyboard.addKeyPressed( 's', [&](EventData event) { cameraScroll->m_y += cameraStep; } );

    engine.input.keyboard.triggerListeners( 'a', KeyEventType::PRESSED );
    return *this;
  }
  
  
  IsometricGame &loop() {
    int HEIGHT = 4;
    int SIDE = 10;

    int mapa[HEIGHT][SIDE*SIDE] = {
      {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
      },
      {
        1,1,1,0,0,0,0,1,1,1,
        1,1,0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,0,0,1,
        0,0,0,0,1,0,0,0,0,0,
        0,0,0,1,1,1,0,0,0,0,
        0,0,1,1,1,1,1,0,0,0,
        0,0,0,1,1,1,0,0,0,0,
        1,0,0,0,1,0,0,0,0,1,
        1,1,0,0,0,0,0,0,1,1,
        1,1,1,0,0,0,0,1,1,1,
      },
      {
        1,1,0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        1,0,0,0,0,0,0,0,0,1,
        1,1,0,0,0,0,0,0,1,1,
      },
      {
        1,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        1,0,0,0,0,0,0,0,0,1,
      },
    };

    int gridLenght = 50;

    for( int axis = 0; axis <= gridLenght; ++axis ) {
      isometric_drawer.drawNormalizedLine(axis, 0, axis, gridLenght, 0xFF00B000);
      isometric_drawer.drawNormalizedLine(0, axis, gridLenght, axis, 0xFFFF0000);
    }

    int offsetMapX = 15;
    int offsetMapY = 15;
    
    for(int Z = 0; Z < HEIGHT; ++Z ) {
      for( int Y = SIDE - 1; Y > -1; --Y ) {
        for( int X = SIDE - 1; X > -1; --X ) {
          int indexCell = Y * SIDE + X;
          int& cell = mapa[Z][ indexCell ];
        
          int frontFaceIsVisible = Y > 0 ? FRONT_FACE * !mapa[Z][ (Y-1) * SIDE + X ] : FRONT_FACE;
          int leftFaceIsVisible = X > 0 ? LEFT_FACE * !mapa[Z][ Y * SIDE + X-1 ] : LEFT_FACE;
          int topFaceIsVisible = Z < HEIGHT-1 ? TOP_FACE * !mapa[Z+1][ indexCell ] : TOP_FACE;
          
          uint32_t color = 0xFFB0B0B0;

          if( Z == 0 ) {
             bool isBlack = (X ^ Y) & 1;
              if(isBlack) color = 0xFF000000;
          }

          if(cell) {
              isometric_drawer.drawFillCube( X + offsetMapX, Y + offsetMapY, Z, 0xFF505050, 0xFF909090, color + 0x00101010 * Z, frontFaceIsVisible | leftFaceIsVisible | topFaceIsVisible);
          }
        }
      }
    }
    
    Vec2i pointer = isometric_drawer.mouseCoordsToNormMap( engine.input.mouse.cursorX, engine.input.mouse.cursorY );
    
    if(pointer.m_x >= 0 && pointer.m_y >= 0) {
      for( int Y = -1; Y <= 1; ++Y ) {
        for( int X = -1; X <= 1; ++X ) {
          int cellX = pointer.m_x + X;
          int cellY = pointer.m_y + Y;
          if( cellX > -1 && cellY > -1 ) {
            isometric_drawer.drawFillCube(cellX, cellY, 0, 0, 0, 0xFFFFFF00, TOP_FACE);
          }
        }
      }
    }
    
    return *this;
  }

  IsometricGame &onRelease() { return *this; }

private:
  IsometricDrawerPipeline isometric_drawer;
  int width = 1920;
  int height = 1080;
};

/**
 * Game Factory used in app initialization.
 */
IsometricGame game_factory() { return IsometricGame(); }

int main(int argc, char **argv) {
  try {
    game_factory()
        .buildEngineInstance()
        .onCreate()
        .onCreateWindow()
        .onInitialize()
        .onStart();

    return 0;
  } catch (int errorCode) {
    std::cerr << "ERROR: Code " << errorCode << "\n";
    return -1;
  } catch (const std::exception &error) {
    return -1;
  }
  return 0;
}
