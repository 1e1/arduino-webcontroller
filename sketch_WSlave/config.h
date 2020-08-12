#ifndef config_H_
#define config_H_

#include "macro.h"


#define MODE_SERIAL             MODE_SERIAL_ALL
#define MODE_VERBOSE            MODE_VERBOSE_AUTO
#define PIN_VISIBILITY_DEFAULT  PIN_VISIBILITY_VISIBLE



// a number between 1 and 254
#define DEVICE_NUMBER   12

// USB conf
#define USB_SPEED       115200

// ETH conf
#define DHCP_TIMEOUT_MS 3000
//#define IP              10,240,170, DEVICE_NUMBER
//#define SUBNET          255,255,255,0
//#define GATEWAY         0,0,0,0
//#define DNS             0,0,0,0
// ascii code for "@lan#" + HEX 12
//#define MAC             MAC_ADDRESS( DEVICE_NUMBER )
#define WEB_PORT        80
#define ETH_BLPIN       53 /* power of W5100 chip */

#endif // config_H_
