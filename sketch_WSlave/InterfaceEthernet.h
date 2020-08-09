#ifndef EngineEthernet_H_
#define EngineEthernet_H_

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Ethernet.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterface.h"
#include "Core.h"
#include "_webApp.h"



#define CRLF              "\r\n"
#define HEADER_START      F("HTTP/1.0 200" CRLF "Content-Type: text/")
#define HEADER_END_ACTION F("plain" CRLF CRLF)
#define HEADER_END_HELP   F("html" CRLF "Content-Encoding: gzip" CRLF CRLF)



class InterfaceEthernet : public AbstractInterface {

  public:
  InterfaceEthernet();

  void begin(); // call it in setup()
  void check(); // call it once in the main loop()
  void work();
  void idle();
  void clean();

  protected:
  EthernetServer* _webServer;

};


#endif
