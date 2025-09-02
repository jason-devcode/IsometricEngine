#include "../include/api/api.hpp"
#include "../include/core/vectorial.hpp"
#include "../include/utils/CoordinateSpace.hpp"
#include "../include/utils/ScreenSpace.hpp"
#include "../include/utils/geometry.hpp"
#include "../include/utils/trigonometry.hpp"

#include "../include/pipelines/isometric_pipeline.hpp"

#include <cstdint>

#include "../include/utils/Texture.hpp"

#include "../include/utils/PPMImage.hpp"

#include "../include/utils/Text.hpp"

using std::uint32_t;

#define MAX_TEXTURES 5

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
  Texture *textures[ MAX_TEXTURES ]{nullptr};
  std::string texturePaths[ MAX_TEXTURES ] = {
    "../assets/cityTiles_066.ppm",
    "../assets/cityTiles_056.ppm",
    "../assets/cityTiles_105.ppm",
    "../assets/cityTiles_081.ppm",
    "../assets/cityTiles_057.ppm"
  };

  int selectorRange = 0;

  IsometricGame &onInitialize() {
    isometric_drawer
      .setGraphics(&engine.graphics)
      .initalize(width, height);

    auto& keyboard = engine.input.keyboard;
    
    cameraScroll = isometric_drawer.getCameraScroll();
    cameraScroll->m_y -= 3;

    constexpr double cameraStep = 3;
    double& deltatime = *engine_timer.getDeltatimeRef();

    keyboard.addKeyPressed( 'a', [&](EventData event) { cameraScroll->m_x += cameraStep * deltatime; } );
    keyboard.addKeyPressed( 'd', [&](EventData event) { cameraScroll->m_x -= cameraStep * deltatime; } );
    keyboard.addKeyPressed( 'w', [&](EventData event) { cameraScroll->m_y -= cameraStep * deltatime; } );
    keyboard.addKeyPressed( 's', [&](EventData event) { cameraScroll->m_y += cameraStep * deltatime; } );
    
    for( int i = 0; i < 9; ++i )
      keyboard.addKeyPressed( '1' + i, [&, i](EventData event) { selectorRange = i; } );
    
    for( int textureIterator = 0; textureIterator < MAX_TEXTURES; ++textureIterator ) {
      textures[ textureIterator ] = PPMImage::readFile(texturePaths[ textureIterator ]);
    }
    
    tout.setOffsets(25, 25).setEnableBg(true);
    tout << std::fixed << std::setprecision(2);
    return *this;
  }
  
  
  IsometricGame &loop() {
    int HEIGHT = 1;
    int SIDE = 10;

    int mapa[HEIGHT][SIDE*SIDE] = {
      {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,2,2,1,1,1,
        1,1,1,1,1,2,2,1,1,1,
        4,4,4,4,5,4,4,4,4,4,
        1,1,1,1,1,1,1,1,1,1,
        3,3,3,3,3,3,3,3,3,3,
      } 
    };
    
    
    
    int gridLenght = 100;

    for( int axis = 0; axis <= gridLenght; ++axis ) {
      isometric_drawer.drawNormalizedLine(axis, 0, axis, gridLenght, 0xFF005000);
      isometric_drawer.drawNormalizedLine(0, axis, gridLenght, axis, 0xFF500000);
    }

    int offsetMapX = 15;
    int offsetMapY = 15;
    
    for(int Z = 0; Z < 1; ++Z ) {
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

          if(cell > 0) {
              isometric_drawer.drawTexturedCube( X + offsetMapX, Y + offsetMapY, Z, textures[ cell - 1 ], frontFaceIsVisible | leftFaceIsVisible | topFaceIsVisible);
          }
        }
      }
    }
    Vec2i pointer = isometric_drawer.mouseCoordsToNormMap( engine.input.mouse.cursorX, engine.input.mouse.cursorY );


    if(pointer.m_x >= 0 && pointer.m_y >= 0) {
      for( int Y = -selectorRange; Y <= selectorRange; ++Y ) {
        for( int X = -selectorRange; X <= selectorRange; ++X ) {
          int cellX = pointer.m_x + X;
          int cellY = pointer.m_y + Y;
          if( cellX > -1 && cellY > -1 && cellX < gridLenght && cellY < gridLenght ) {
            isometric_drawer.drawFillCube(cellX, cellY, 0, 0, 0, 0xFFFFFF00, TOP_FACE);
          }
        }
      }
    }
    
    tout << "Camera X: " << cameraScroll->m_x << "\n";
    tout << "Camera Y: " << cameraScroll->m_y << "\n";
    tout << "Cursor Cell X: " << pointer.m_x << "\n";
    tout << "Cursor Cell Y: " << pointer.m_y << "\n";
    
    tout.render();

    return *this;
  }

  IsometricGame &onRelease() { 
    std::cout << "IsometricGame::onRelease()\n";
    for( auto* texture : textures ) delete texture;
    return *this; 
  }

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
