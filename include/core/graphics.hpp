#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

namespace core {

  

};

#include <cstdint>

class CGame {
  public:
    explicit CGame() {}
    ~CGame() {}

    static CGame& createGame() noexcept { return CGame(); }

    // use builder pattern to initialize game properties

    CGame& 

    CGame& setScreenDims( std::uint32_t widthPx, std::uint32_t heighPx ) {

      return *this;
    }

  private:

};


#endif
