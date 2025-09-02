#ifndef CORE_TIMERS_HPP
#define CORE_TIMERS_HPP

/** for */
#include <chrono>

/** for std::cout */
#include <iostream>

/**
 * Use hight_resolution_clock for more precise tick clock time representation
 * */

using namespace std;

class Timers {
  public:
    Timers() {
      cout << "Timers::Timers()\n";
    }
    ~Timers() {
      cout << "Timers::~Timers()\n";
    }

    void startFrame() {
      start = chrono::high_resolution_clock::now();
    }

    void calculateDeltatime() {
      end = chrono::high_resolution_clock::now();
      chrono::duration<double> dt = end - start;
      deltatime = dt.count();
    }
    double* getDeltatimeRef() {
      return &deltatime;
    }
  private:
    double deltatime;
    chrono::time_point<chrono::high_resolution_clock> start, end;

};

Timers engine_timer;

#endif // CORE_TIMERS_HPP
