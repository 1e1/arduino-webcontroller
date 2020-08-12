#ifndef AbstractStream_h
#define AbstractStream_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterface.h"
#include "Core.h"



#define ACTION_ALL          '*'
#define ACTION_GET          'G'
#define ACTION_PUT          'P'
#define ACTION_DELETE       'D'
#define PATH_SEPARATOR      '/'
#define LF                  '\n'
#define TEXT_INPUT          F(" IN ")
#define TEXT_OUTPUT         F(" OUT ")
#define CMD_HIDE            '.'
#define CMD_ALL             '$'
#define PREFIX_DIGITAL_PIN  'D'
#define PREFIX_PWM_PIN      'P'
#define PREFIX_ANALOG_PIN   'A'
#define PIN_NONE            -1
#define BUFFER_SIZE         8




class AbstractStream : public AbstractInterface {

  public:
  bool read();
  void process();
  void terminate();

  // inline
  __attribute__((always_inline)) inline void setStream(Stream* inputStream) { this->_currentStream = inputStream; };

  protected:

  void _showOne(const uint8_t pin);
  void _showAll();

  bool _findUntil(const int terminator, size_t length);

  Stream* _currentStream;

  char _currentAction;
  uint8_t _currentPin;

};



#endif
