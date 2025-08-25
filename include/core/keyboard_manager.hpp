#ifndef KEYBOARD_MANAGER_HPP
#define KEYBOARD_MANAGER_HPP

/** for std::cout */
#include <iostream>

/** for std::function */
#include <functional>

/** for std::vector */
#include <vector>

/** std::unordered_map */
#include <unordered_map>

using std::vector;
using std::function;
using std::unordered_map;

class EventData {};

enum class KeyEventType {
  PRESSED,
  KEYDOWN,
  KEYUP,
};

using KeyCode = int;

using ListenerFunction = function<void(EventData&)>;

using ListenerList = 
  unordered_map<KeyCode, vector<ListenerFunction>>;

using KeyboardListenerLists =
  unordered_map< KeyEventType, ListenerList >;


class KeyboardManager {
  public:
    KeyboardManager() {
      std::cout << "KeyboardManager::KeyboardManager()\n";
    }
    ~KeyboardManager() {
      std::cout << "KeyboardManager::~KeyboardManager()\n";
    }

    void addKeyPressed( KeyCode key,  ListenerFunction listener ) {
      vector<ListenerFunction>& list = listenersLists[ KeyEventType::PRESSED ][ key ];
      list.push_back( listener );
    }

    void triggerListeners( KeyCode key, KeyEventType type ) {
      EventData event;
      for( auto& listener : listenersLists[ type ][ key ] ) {
        listener(event);
      }
    }

    void triggerPressedKeyListeners() {
        // iterate all pressed keys
        for( const auto& [key, state] : keyStates ) {
          if(state)
            triggerListeners( key, KeyEventType::PRESSED );
        }
    }
  
    bool getKeyState( KeyCode key ) {
      auto it = keyStates.find( key );
      return (it != keyStates.end()) ? it->second : false;
    }

    void setKeyState( KeyCode key, bool state ) {
      keyStates[ key ] = state;
    }
  private:
    unordered_map< KeyCode, bool > keyStates;
    
    KeyboardListenerLists listenersLists = { 
      { KeyEventType::PRESSED, {} },
      { KeyEventType::KEYDOWN, {} },
      { KeyEventType::KEYUP, {} },
    };
};

#endif
