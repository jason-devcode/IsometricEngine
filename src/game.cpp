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
        .setScreenColor(0xFF000000)
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

    constexpr double cameraStep = 0.009;

    keyboard.addKeyPressed( 'a', [&](EventData event) { cameraScroll->m_x += cameraStep; } );
    keyboard.addKeyPressed( 'd', [&](EventData event) { cameraScroll->m_x -= cameraStep; } );
    keyboard.addKeyPressed( 'w', [&](EventData event) { cameraScroll->m_y -= cameraStep; } );
    keyboard.addKeyPressed( 's', [&](EventData event) { cameraScroll->m_y += cameraStep; } );

    engine.input.keyboard.triggerListeners( 'a', KeyEventType::PRESSED );
    return *this;
  }
  
  
  IsometricGame &loop() {
    const double axisLength = 25;
    
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
    
    for(int Z = 0; Z < HEIGHT; ++Z ) {
      for( int Y = SIDE - 1; Y > -1; --Y ) {
        for( int X = SIDE - 1; X > -1; --X ) {
          int indexCell = Y * SIDE + X;
          int& cell = mapa[Z][ indexCell ];
        
          int frontFaceIsVisible = Y > 0 ? FRONT_FACE * !mapa[Z][ (Y-1) * SIDE + X ] : FRONT_FACE;
          int leftFaceIsVisible = X > 0 ? LEFT_FACE * !mapa[Z][ Y * SIDE + X-1 ] : LEFT_FACE;
          int topFaceIsVisible = Z < HEIGHT-1 ? TOP_FACE * !mapa[Z+1][ indexCell ] : TOP_FACE;

          if( Z == 0 ) {
             
          }

          if(cell) {
              isometric_drawer.drawFillCube(X,Y,Z, 0xFF505050, 0xFF909090, 0xFFB0B0B0 + 0x00101010 * Z, frontFaceIsVisible | leftFaceIsVisible | topFaceIsVisible);
          }
        }
      }
    }
    return *this;
  }

  IsometricGame &onRelease() { return *this; }

private:
  IsometricDrawerPipeline isometric_drawer;
  int width = 1280;
  int height = 720;
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
