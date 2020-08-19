#ifndef Core_h
#define Core_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"



class Core {

  public:
  static __attribute__((always_inline)) inline bool isPinAnalog(const uint8_t pin) { return pin >= NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS; };
  static __attribute__((always_inline)) inline bool isPinPwm(const uint8_t pin) { return digitalPinHasPWM(pin); };
  static uint8_t getPinMode(const uint8_t pin);

  static const bool isPinVisible(const uint8_t pin);
  static void isPinVisible(const uint8_t pin, const bool isVisible);

  static const int get(const uint8_t pin);
  static const int get(const uint8_t pin, const uint8_t mode, const bool isAnalog);
  static void set(const uint8_t pin, const uint8_t value);
  static void unset(const uint8_t pin);

  protected:
  struct option {
    option(): isVisible(PIN_VISIBILITY_DEFAULT) {};
    bool isVisible : 1;
  };

  static uint8_t _outputValues[NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS];
  static option _options[NUM_DIGITAL_PINS];

};



#endif
