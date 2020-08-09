#ifndef LedBuiltIn_H_
#define LedBuiltIn_H_

#include <Arduino.h>
#include "AbstractInterface.h"



#ifdef LED_BUILTIN
  #define BUSYLED_HIGH analogWrite(LED_BUILTIN, 255)
  #define BUSYLED_WORK analogWrite(LED_BUILTIN, 63)
  #define BUSYLED_IDLE analogWrite(LED_BUILTIN, 15)
  #define BUSYLED_NONE analogWrite(LED_BUILTIN, 0)
#else
  #define BUSYLED_HIGH
  #define BUSYLED_WORK
  #define BUSYLED_IDLE
  #define BUSYLED_NONE
#endif



class LedBuiltIn : public AbstractInterface {

  public:
  typedef enum { OFF=0, IDLE=15, WORK=63, ON=255 } State;

  LedBuiltIn(const byte pin);
  __attribute__((always_inline)) inline void state(const State value) { analogWrite(this->_pin, value); };

  inline void check() { this->state(State::ON  ); };
  inline void work()  { this->state(State::WORK); };
  inline void idle()  { this->state(State::IDLE); };
  inline void clean() { this->state(State::OFF ); };

  protected:
  byte _pin;

};


#endif
