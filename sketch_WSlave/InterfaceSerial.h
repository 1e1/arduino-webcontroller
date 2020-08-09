#ifndef InterfaceSerial_H_
#define InterfaceSerial_H_

#include <Arduino.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterface.h"
#include "Core.h"



class InterfaceSerial : public AbstractInterface {

  public:
  void begin(); // call it in setup()
  void check(); // call it once in the main loop()
  void work();
  void idle();
  void clean();

  protected:
  Stream* _stream;

};


#endif
