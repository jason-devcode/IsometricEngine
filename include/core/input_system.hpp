#ifndef CORE_INPUT_SYSTEM_HPP
#define CORE_INPUT_SYSTEM_HPP

#include "keyboard_manager.hpp"

#include <iostream>

class InputSystem {
  public:
    InputSystem() {
      std::cout << "InputSystem::InputSystem()\n";
    }
    ~InputSystem() {
      std::cout << "InputSystem::~InputSystem()\n";
    }

    KeyboardManager keyboard;
};

#endif
