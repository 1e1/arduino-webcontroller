#ifndef config_H_
#define config_H_

#include "macro.h"


#define MODE_SERIAL_AUTO        0
#define MODE_SERIAL_USB         1
#define MODE_SERIAL_ETHERNET    2
#define MODE_PRO 0

// a number between 1 and 254
#define DEVICE_NUMBER       12
#define DEVICE_NAME_PREFIX  "WSlave_"

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
