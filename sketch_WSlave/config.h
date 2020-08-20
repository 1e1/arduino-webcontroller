#ifndef config_H_
#define config_H_

#include "Arduino.h"
#include "macro.h"


// MODE_SERIAL = MODE_SERIAL_USB | MODE_SERIAL_ETHERNET | MODE_SERIAL_LCD | MODE_SERIAL_ALL
#define MODE_SERIAL             MODE_SERIAL_USB + MODE_SERIAL_ETHERNET

// MODE_VERBOSE = MODE_VERBOSE_NONE | MODE_VERBOSE_HELP | MODE_VERBOSE_LIST | MODE_VERBOSE_WEBAPP | MODE_VERBOSE_ALL
#define MODE_VERBOSE            MODE_VERBOSE_ALL

// PIN_VISIBILITY_DEFAULT = PIN_VISIBILITY_HIDDEN | PIN_VISIBILITY_VISIBLE
#define PIN_VISIBILITY_DEFAULT  PIN_VISIBILITY_VISIBLE

// TYPE_MAC = TYPE_MAC_STATIC |TYPE_MAC_RANDOM
#define TYPE_MAC                TYPE_MAC_RANDOM



// a number between 2 and 253
// =====================
#define DEVICE_NUMBER   12
// =====================


// USB conf
// =====================
#define USB_SPEED       9600
// =====================


// ETH conf
// =====================
#define DHCP_TIMEOUT_MS 3000
#define WEB_PORT        80
// =====================


// LCD conf
// =====================
#define LCD_WIDTH       16
#define LCD_HEIGHT      2
#define LCD_PIN_BL      11  /* backlight /!\ Pin10: CONFLICT on W5100 Ethernet Shield */
#define LCD_PIN_RS      8
#define LCD_PIN_ENABLE  9
#define LCD_PIN_D4      4
#define LCD_PIN_D5      5
#define LCD_PIN_D6      6
#define LCD_PIN_D7      7
#define LCD_PINS        LCD_PIN_RS, LCD_PIN_ENABLE, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7
#define LCD_PIN_ABTN    analogInputToDigitalPin(0)   /* analog pin of buttons */
// =====================


#endif // config_H_
