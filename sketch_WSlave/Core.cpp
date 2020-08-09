#include "Core.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




Stream* Core::_currentStream;
uint8_t Core::_outputValues[NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS];
volatile Core::option Core::_options[NUM_DIGITAL_PINS]; 

char Core::_currentAction;
uint8_t Core::_currentPin;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * ACTION /(<pin>\d+(/<value>\d+)?)? (...)
 * ACTION ::= G(ET) | P(UT) | D(ELETE)
 * */
bool Core::check()
{
    Core::_currentAction = Core::_currentStream->read();
    Core::_currentPin = PIN_NONE;

    if (Core::_findUntil(PATH_SEPARATOR, BUFFER_SIZE)) {
        const char nextChar = Core::_currentStream->peek();

        if (CMD_ALL == nextChar) {
            Core::_currentAction = ACTION_ALL;

            return true;
        }

        if ('0' <= nextChar && nextChar <= '9') {
            Core::_currentPin = Core::_currentStream->parseInt(SKIP_NONE);

            return true;
        }
    }

    return false;
}



void Core::process()
{
    switch (Core::_currentAction) {

    case ACTION_GET:
        break;

    case ACTION_ALL:
        Core::showAll();
        return;

    case ACTION_DELETE:
        Core::_unset(Core::_currentPin);
        break;

    case ACTION_PUT:
        {
            const char nextChar = Core::_currentStream->read(); // jump the separator
            
            if (PATH_SEPARATOR == nextChar) {
                const uint8_t value = Core::_currentStream->parseInt(SKIP_NONE);
                Core::_set(Core::_currentPin, value);
            } else {
                #if MODE_PRO == 0
                Core::_isPortVisible(Core::_currentPin, CMD_HIDE != nextChar);
                #endif
            }
        }
        break;
    }

    Core::showOne(Core::_currentPin);
}



void Core::terminate()
{
    Core::_findUntil(LF, -1);
    Core::_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void Core::showOne(const uint8_t pin)
{
    const bool isAnalog = pin >= NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS;
    const bool isPwm = digitalPinHasPWM(pin);

    Core::_currentStream->print(isAnalog ? PREFIX_ANALOG_PIN : isPwm ? PREFIX_PWM_PIN : PREFIX_DIGITAL_PIN);
    Core::_currentStream->print(pin);

    const uint8_t mode = Core::_getPinMode(pin);
    const int value = Core::_get(pin, mode, isAnalog || isPwm);

    if (OUTPUT == mode) {
        Core::_currentStream->print(TEXT_INPUT);
    } else {
        Core::_currentStream->print(TEXT_OUTPUT);
    }

    Core::_currentStream->println(value);
}


void Core::showAll()
{
   uint8_t i = NUM_DIGITAL_PINS;
   while (i-->0) {
        if (Core::_isPortVisible(i)) {
            Core::showOne(i);
        }
   }
}



uint8_t Core::_getPinMode(const uint8_t pin)
{
    /*
    // Check valid pin number
    if (pin >= NUM_ANALOG_INPUTS + NUM_DIGITAL_PINS) {
        return -1;
    }
    */
    // Convert designated Arduino pin to ATMega port and pin
    uint8_t pbit  = digitalPinToBitMask(pin);
    uint8_t pport = digitalPinToPort(pin);

    // Read the ATmega DDR for this port
    volatile uint8_t *pmodereg = portModeRegister(pport);

    // Test the value of the bit for this pin and return
    // 0 if it is reset and 1 if it is set
    return ((*pmodereg & pbit) != 0);
}



bool Core::_findUntil(const int terminator, size_t length)
{
    do {
        delay(1);
    }
    while (terminator!=Core::_currentStream->read() && --length);

    return (bool) length;
}



const bool Core::_isPortVisible(const uint8_t pin)
{
    return Core::_options[pin].isVisible;
}



void Core::_isPortVisible(const uint8_t pin, const bool isVisible)
{
    Core::_options[pin].isVisible = isVisible;
}


const int Core::_get(const uint8_t pin, const uint8_t mode, const bool isAnalog)
{
   if (OUTPUT == mode) {
       return Core::_outputValues[pin];
   }

   if (isAnalog) {
       return analogRead(pin);
   }

   return digitalRead(pin);
}


 
void Core::_set(const uint8_t pin, const uint8_t value) {
    Core::_outputValues[pin] = value;
    analogWrite(pin, value);
}


 
void Core::_unset(const uint8_t pin) {
    pinMode(pin, INPUT);
}
