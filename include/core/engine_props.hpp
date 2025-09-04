#ifndef CORE_ENGINE_PROPS_HPP
#define CORE_ENGINE_PROPS_HPP

/** for std::cout */
#include <iostream>

/** for IGame */
#include "../api/igame.hpp"

class EngineProps {
  public:
    EngineProps() {
      std::cout << "EngineProps::EngineProps()\n";
    }
    ~EngineProps() {
      std::cout << "EngineProps::~EngineProps()\n";
    }

    void setInstanceProps( 
      std::string title, 
      uint32_t width, 
      uint32_t height, 
      uint32_t color, 
      bool vsyncEnable, 
      int fpsLimit, 
      IGame *currentGame 
    ) {
      m_title = title;
      m_screenWidth_px = width;
      m_screenHeight_px = height;
      m_screenColor = color;
      m_vsyncEnable = vsyncEnable;
      m_vsyncLimit = fpsLimit;
      m_currentGame = currentGame;
    } 

    std::string m_title = "Default App";
    
    uint32_t m_screenWidth_px = 256;
    uint32_t m_screenHeight_px = 256;
    uint32_t m_screenColor = 0xFF000000;

    uint32_t m_vsyncLimit = 30;
    bool m_vsyncEnable = true;
    
    IGame* m_currentGame = nullptr;
};

#endif // CORE_ENGINE_PROPS_HPP
