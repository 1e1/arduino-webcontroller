#include "InterfaceSerial.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void InterfaceSerial::begin()
{
  Serial.begin(USB_SPEED);
}


void InterfaceSerial::check()
{
  Core::setStream(&Serial);

  if (Serial.available() > 0) {
    if (Core::check()) {
      Core::process();
    } else {
      #if MODE_PRO == 0
      Serial.println(TEXT_HELP);
      #endif
    }

    Core::terminate();
  }
}


void InterfaceSerial::work()
{
  // Serial.println(F("IDLE"));
}


void InterfaceSerial::idle()
{
}


void InterfaceSerial::clean()
{
}
