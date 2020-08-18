#ifndef InterfaceLcd1602_H_
#define InterfaceLcd1602_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterface.h"
#include "Core.h"


/**
  *
  *   01234567890ABCDE
  *  +----------------+
  * 0|PIN <BAR_8>=####|0
  * 1|XXXXXXXXXXXXXXXX|1
  *  +----------------+
  *   01234567890ABCDE
  *
  *   01234567890ABCDE
  *  +----------------+
  * 0|PIN <BAR_8>=####|0
  *  +----------------+
  *   01234567890ABCDE
  *
  *   01234567
  *  +--------+
  * 0|PIN=####|0
  *  +--------+
  *   01234567
  *
  * PIN: Digital:'D' | Analog:'A' + {pin_id:[0-99]}
  * PWM: PWM:'PWM' | *:'   '
  * M: input | output
  * =: input:'<' | output:'>'
  * ###: value [0-999]
  * X: 1-char state
  *
  * UP/DOWN: Core::_unset() / this->_next()
  * LEFT/RIGHT: Core::_set(-16) / Core::_set(+16)
  * SELECT: switch() = Core::_set(LOW) | Core::_set(HIGH)
  *
  */



#define LCDCHAR(ch) static const uint8_t ch[] PROGMEM

// LCD conf
#define LCD_BACKLIGHT_ON    128
#define LCD_BACKLIGHT_SLEEP 240
#define LCD_BACKLIGHT_OFF   255
#define LCDPAD_LATENCY1_MS  10    // detect press down
#define LCDPAD_LATENCYX_MS  500   // switch mode from ONCE to MULTI
#define LCDPAD_SPEED_MS     200   // considering new press down

#define KEYPAD_MAXVALUE_RIGHT       /* * / 30  /* * / 50  /* */ 0
#define KEYPAD_MAXVALUE_UP          /* * / 150 /* * / 200 /* */ 144
#define KEYPAD_MAXVALUE_DOWN        /* * / 360 /* * / 400 /* */ 329
#define KEYPAD_MAXVALUE_LEFT        /* * / 535 /* * / 600 /* */ 505
#define KEYPAD_MAXVALUE_SELECT      /* * / 760 /* * / 800 /* */ 742
#define KEYPAD_THRESHOLD            5

#define CONTROL_VALUE_DELTA         (256/LCD_WIDTH)
#define DISPLAY_PREFIX_DIGITAL_PIN  'D'
#define DISPLAY_PREFIX_ANALOG_PIN   'A'
// #define DISPLAY_MODE_OUTPUT         '>'
#define DISPLAY_MODE_OUTPUT         ((char) 0x7E)
// #define DISPLAY_MODE_INPUT          '<'
#define DISPLAY_MODE_INPUT          ((char) 0x7F)
//#define DISPLAY_PB_VOID             ' '
#define DISPLAY_PB_VOID             ((char) 0xA5)
// #define DISPLAY_PB_FULL             '='
#define DISPLAY_PB_FULL             ((char) 0xFF)



class InterfaceLcd1602 : public AbstractInterface {

  public:
  InterfaceLcd1602();
  InterfaceLcd1602(LiquidCrystal* lcd);

  void begin(); // call it in setup()
  void loop();  // call it once in the main loop()
  void raise();
  void reset();

  typedef enum { SHUTDOWN=0, SLEEPING, IDLE, AWAKE } State;
  typedef enum { KEYPAD_NONE, KEYPAD_RIGHT, KEYPAD_UP, KEYPAD_DOWN, KEYPAD_LEFT, KEYPAD_SELECT } Key;

  protected:
  void _input();
  void _output();
  void _outputExtra();
  void _reset();
  void _next();
  void _switchValue();
  void _changeValue(const int8_t delta);
  void _updateBacklight();
  void _printEmptyBar(uint8_t size);
  void _printProgressBar(const uint8_t size, const uint8_t value);

  const boolean _hasNewPulsedKey();
  const Key _getKey();

  LiquidCrystal* _lcd;
  uint8_t _pin;
  State   _state;
  Key     _key;
  Key     _tmpKey;

};


#endif
