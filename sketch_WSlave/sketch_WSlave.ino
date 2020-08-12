/*
 Rest Server

 https://github.com/1e1/WSlave

 Circuit at least one:
 * Ethernet shield
 * LCD shield

 created 29 Aug 2012
 by Aymeric GERLIER
 modified 4 Sep 2012
 by Aymeric GERLIER

 */



#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Ethernet.h>
#include "macro.h"
#include "config.h"
#include "InterfaceEthernet.h"
#include "InterfaceSerial.h"
#include "FastTimer.h"
#include "LedBuiltIn.h"



/** ===================== **/
/**       observers       **/
/** ===================== **/

// Restarts program from beginning but does not reset the peripherals and registers
__attribute__((always_inline)) inline void software_reset();
void software_reset()
{
  asm volatile ("jmp 0");
}




/** ===================== **/



static AbstractInterface* _engine;
static FastTimer* _timer;




void setup()
{
  BUSYLED_HIGH;
  #if MODE_SERIAL & MODE_SERIAL_ETHERNET
  randomSeed(analogRead(0));
  #endif

  const FastTimer::Precision p = FastTimer::Precision::P65s_4h;
  _timer = new FastTimer(p);

  #if MODE_SERIAL & MODE_SERIAL_ETHERNET
  const uint8_t deviceNumber = random(2, 253);
  byte mac[] = MAC_ADDRESS(deviceNumber);
  #endif

  // reduce DHCP timeout, default is 60000ms
  // change: Ethernet.cpp/EthernetClass::begin{...int ret = _dhcp->beginWithDHCP(mac_address);...}
  // by:     Ethernet.cpp/EthernetClass::begin{...int ret = _dhcp->beginWithDHCP(mac_address, 10000);...}
  // reduce number of client sockets to only one master (break loops)
  // change: Ethernet.h/#define MAX_SOCK_NUM 4
  // by:     Ethernet.h/#define MAX_SOCK_NUM 1

  #if MODE_SERIAL == MODE_SERIAL_AUTO
  if (1 == Ethernet.begin(mac, DHCP_TIMEOUT_MS)) {
    _engine = new InterfaceEthernet();
  } else {
    _engine = new InterfaceSerial();
  }
  #elif MODE_SERIAL == MODE_SERIAL_ETHERNET
  _engine = new InterfaceEthernet();
  #else
  _engine = new InterfaceSerial();
  #endif

  BUSYLED_NONE;

  _engine->begin();
}


void loop()
{
  const bool isTick = _timer->update();

  if (isTick) {
    BUSYLED_WORK;
    // broadcast
    _engine->work();

    if (0 == _timer->getTime()) {
      // maintain
      _engine->idle();
    }
  }

  BUSYLED_IDLE;
  _engine->check();

  BUSYLED_NONE;
  _engine->clean();
}


void fail()
{
#ifdef LED_BUILTIN
  uint8_t light = 0;
  for(;;) {
    analogWrite(LED_BUILTIN, light--);
    delay(5);
  }
#endif
}
