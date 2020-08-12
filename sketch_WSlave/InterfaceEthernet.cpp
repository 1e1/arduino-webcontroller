#include "InterfaceEthernet.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




LONGBYTES(webpage)        = WEBPAGE;
static size_t webpage_len = ARRAYLEN(webpage);




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceEthernet::InterfaceEthernet()
{
  this->_webServer = new EthernetServer(WEB_PORT);
}




void InterfaceEthernet::begin()
{
  this->_webServer->begin();
}


void InterfaceEthernet::check()
{
  EthernetClient client = this->_webServer->available();

  Core::setStream(&client);

  if (client.connected()) {
    if (client.available()) {

      #if MODE_VERBOSE == MODE_VERBOSE_PRO
      if (Core::check()) {
        client.print(HEADER_START HEADER_END_ACTION);
        Core::process();
      }
      #else
      client.print(HEADER_START);

      if (Core::check()) {
        client.print(HEADER_END_ACTION);
        Core::process();
      } else {
        client.print(HEADER_END_HELP);
        for (int i = 0; i<webpage_len; i++) {
          client.write(pgm_read_byte_near(webpage + i));
        }
      }
      #endif

      Core::terminate();
    }

    client.stop();
  }
}


void InterfaceEthernet::work()
{
}


void InterfaceEthernet::idle()
{
  Ethernet.maintain();
}


void InterfaceEthernet::clean()
{
}