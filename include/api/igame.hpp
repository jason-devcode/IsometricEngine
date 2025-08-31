#ifndef ABSTRACT_INTERFACE_GAME_HPP
#define ABSTRACT_INTERFACE_GAME_HPP

class IGame {
  public:
    IGame(){}
    ~IGame(){}
    
    virtual IGame& loop() = 0;
    virtual IGame& onRelease() = 0;
};

#endif
