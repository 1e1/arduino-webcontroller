#include "AbstractStream.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/









/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * ACTION /(<pin>\d+(/<value>\d+)?)? (...)
 * ACTION ::= G(ET) | P(UT) | D(ELETE)
 * */
bool AbstractStream::read()
{
    this->_currentAction = this->_currentStream->read();
    this->_currentPin = PIN_NONE;

    if (this->_findUntil(PATH_SEPARATOR, BUFFER_SIZE)) {
        const char nextChar = this->_currentStream->peek();

        if (CMD_ALL == nextChar) {
            this->_currentAction = ACTION_ALL;

            return true;
        }

        if ('0' <= nextChar && nextChar <= '9') {
            this->_currentPin = this->_currentStream->parseInt(SKIP_NONE);

            return true;
        }
    }

    return false;
}



void AbstractStream::process()
{
    switch (this->_currentAction) {

    case ACTION_GET:
        break;

    case ACTION_ALL:
        this->_showAll();
        return;

    case ACTION_DELETE:
        Core::unset(this->_currentPin);
        break;

    case ACTION_PUT:
        {
            const char nextChar = this->_currentStream->read(); // jump the separator

            if (PATH_SEPARATOR == nextChar) {
                const uint8_t value = this->_currentStream->parseInt(SKIP_NONE);
                Core::set(this->_currentPin, value);
            } else {
                #if MODE_PRO == 0
                Core::isPinVisible(this->_currentPin, CMD_HIDE != nextChar);
                #endif
            }
        }
        break;
    }

    this->_showOne(this->_currentPin);
}



void AbstractStream::terminate()
{
    this->_findUntil(LF, -1);
    this->_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void AbstractStream::_showOne(const uint8_t pin)
{
    const bool isAnalog = Core::isPinAnalog(pin);
    const bool isPwm = Core::isPinPwm(pin);

    this->_currentStream->print(isAnalog ? PREFIX_ANALOG_PIN : isPwm ? PREFIX_PWM_PIN : PREFIX_DIGITAL_PIN);
    this->_currentStream->print(pin);

    const uint8_t mode = Core::getPinMode(pin);
    const int value = Core::get(pin, mode, isAnalog || isPwm);

    if (OUTPUT == mode) {
        this->_currentStream->print(TEXT_INPUT);
    } else {
        this->_currentStream->print(TEXT_OUTPUT);
    }

    this->_currentStream->println(value);
}


void AbstractStream::_showAll()
{
   uint8_t i = NUM_DIGITAL_PINS;
   while (i-->0) {
        if (Core::isPinVisible(i)) {
            this->_showOne(i);
        }
   }
}



bool AbstractStream::_findUntil(const int terminator, size_t length)
{
    do {
        delay(1);
    }
    while (terminator!=this->_currentStream->read() && --length);

    return (bool) length;
}
