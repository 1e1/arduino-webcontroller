#ifndef Core_h
#define Core_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"



#define ACTION_ALL          '*'
#define ACTION_GET          'G'
#define ACTION_PUT          'P'
#define ACTION_DELETE       'D'
#define PATH_SEPARATOR      '/'
#define LF                  '\n'
#define TEXT_INPUT          F(" INPUT ")
#define TEXT_OUTPUT         F(" OUTPUT ")
#define CMD_HIDE            '.'
#define CMD_ALL             '$'
#define PREFIX_DIGITAL_PIN  'D'
#define PREFIX_PWM_PIN      'P'
#define PREFIX_ANALOG_PIN   'A'
#define PIN_NONE            -1
#define BUFFER_SIZE         8




class Core {

  public:
  static bool check();
  static void process();
  static void terminate();

  // inline
  __attribute__((always_inline)) inline static void setStream(Stream* inputStream) { Core::_currentStream = inputStream; };

  protected:
  struct option {
    option(): isVisible(true) {};
    bool isVisible : 1;
  };

  static void showOne(const uint8_t pin);
  static void showAll();

  static uint8_t _getPinMode(const uint8_t pin);
  static bool _findUntil(const int terminator, size_t length);

  static const bool _isPortVisible(const uint8_t pin);
  static void _isPortVisible(const uint8_t pin, const bool isVisible);

  static const int _get(const uint8_t pin, const uint8_t mode, const bool isAnalog);
  static void _set(const uint8_t pin, const uint8_t value);
  static void _unset(const uint8_t pin);

  static Stream* _currentStream;
  static uint8_t _outputValues[NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS];
  static volatile option _options[NUM_DIGITAL_PINS];

  static char _currentAction;
  static uint8_t _currentPin;

};



#endif
