#include "Core.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




uint8_t Core::_outputValues[NUM_DIGITAL_PINS-NUM_ANALOG_INPUTS];
volatile Core::option Core::_options[NUM_DIGITAL_PINS];





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



uint8_t Core::getPinMode(const uint8_t pin)
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



const bool Core::isPinVisible(const uint8_t pin)
{
    return Core::_options[pin].isVisible;
}



void Core::isPinVisible(const uint8_t pin, const bool isVisible)
{
    Core::_options[pin].isVisible = isVisible;
}


const int Core::get(const uint8_t pin)
{
    Core::get(pin, Core::getPinMode(pin), Core::isPinAnalog(pin) || Core::isPinPwm(pin));
}


const int Core::get(const uint8_t pin, const uint8_t mode, const bool isAnalog)
{
    if (OUTPUT == mode) {
        return Core::_outputValues[pin];
    }

    if (isAnalog) {
        return analogRead(pin);
    }

    return digitalRead(pin);
}



void Core::set(const uint8_t pin, const uint8_t value) {
    Core::_outputValues[pin] = value;
    analogWrite(pin, value);
}



void Core::unset(const uint8_t pin) {
    pinMode(pin, INPUT);
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




