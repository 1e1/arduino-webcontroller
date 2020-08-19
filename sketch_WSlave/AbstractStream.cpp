#include "AbstractStream.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/









/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




/**
 * (...) /<action>\\w /(<pin>\d+( /<value>\d+)?)? (...)
 * */
bool AbstractStream::read()
{
    if (this->_findUntil(PATH_SEPARATOR, BUFFER_SIZE)) {
        this->_currentAction = this->_currentStream->read();
        this->_currentPin = this->_currentStream->parseInt(SKIP_ALL);

        return true;
    }


    return false;
}



void AbstractStream::process()
{
    switch (this->_currentAction) {

        #if MODE_VERBOSE & MODE_VERBOSE_LIST
        case ACTION_ALL:
            this->_showAll();
            return;
        #endif

        case ACTION_INPUT:
            Core::set(this->_currentPin, LOW);
            break;

        case ACTION_OUTPUT:
            Core::unset(this->_currentPin);
            break;
        
        #if MODE_VERBOSE & MODE_VERBOSE_LIST
        case ACTION_HIDE:
        case ACTION_SHOW:
            Core::isPinVisible(this->_currentPin, this->_currentAction == ACTION_SHOW);
            break;
        #endif

        case ACTION_WRITE:
            Core::set(this->_currentPin, this->_currentStream->parseInt(SKIP_ALL));
            break;

        case ACTION_READ:;
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

    if (OUTPUT == mode) {
        this->_currentStream->print(TEXT_OUTPUT);
    } else {
        this->_currentStream->print(TEXT_INPUT);
    }
    
    const int value = Core::get(pin);

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
