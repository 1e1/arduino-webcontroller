#include "InterfaceLcd1602.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



// 1 = DIGITAL|ANALOG=|char|
// 2 = maxPinId=|99]
#define DISPLAY_LEFT_TEXT_LENGTH    (1 + 2)
// 1 = INPUT/OUTPUT=|<char>|
// 4 = maxValue=|1024|
#define DISPLAY_RIGHT_TEXT_LENGTH   (1 + 4)
#define DISPLAY_PROGRESS_BAR_LENGTH (LCD_WIDTH - DISPLAY_LEFT_TEXT_LENGTH - DISPLAY_RIGHT_TEXT_LENGTH)



/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceLcd1602::InterfaceLcd1602()
{
    this->_lcd = new LiquidCrystal(LCD_PINS);
}


InterfaceLcd1602::InterfaceLcd1602(LiquidCrystal* lcd)
{
    this->_lcd = lcd;
}


void InterfaceLcd1602::begin()
{
    this->_state = SHUTDOWN;
    this->_key = KEYPAD_NONE;
    this->_tmpKey = KEYPAD_NONE;

    this->_lcd->begin(LCD_WIDTH, LCD_HEIGHT);

    this->_reset();
}


void InterfaceLcd1602::loop()
{
    if (this->_hasNewPulsedKey() && this->_key != KEYPAD_NONE) {
        this->_state = AWAKE;
        this->_updateBacklight();

        #ifdef LCD_PIN_ABTN
        this->_input();
        #endif
        this->_output();
    }
}


void InterfaceLcd1602::raise()
{
    if (this->_state != SHUTDOWN) {
        this->_state = State(this->_state - 1);
        this->_updateBacklight();
    }

    #if LCD_HEIGHT > 1
    this->_outputExtra();
    #endif
}


void InterfaceLcd1602::reset()
{
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void InterfaceLcd1602::_input()
{
    switch (this->_key) {
        case KEYPAD_UP:
            Core::unset(this->_pin);
            break;

        case KEYPAD_DOWN:
            this->_next();
            break;

        case KEYPAD_LEFT:
            this->_changeValue(-CONTROL_VALUE_DELTA);
            break;

        case KEYPAD_RIGHT:
            this->_changeValue(+CONTROL_VALUE_DELTA);
            break;

        case KEYPAD_SELECT:
            this->_switchValue();
            break;
    }
}


void InterfaceLcd1602::_output()
{
    const bool isAnalog = Core::isPinAnalog(this->_pin);

    // print LEFT_TEXT
    this->_lcd->setCursor(0, 0);
    this->_lcd->print(isAnalog ? DISPLAY_PREFIX_ANALOG_PIN : DISPLAY_PREFIX_DIGITAL_PIN);
    this->_lcd->print(this->_pin);
    if (this->_pin < 10) {
        this->_lcd->print(' ');
    }

    int value = Core::get(this->_pin);

    // print/clear PROGRESS_BAR
    #if DISPLAY_PROGRESS_BAR_LENGTH > 0
    //this->_lcd->setCursor(0, DISPLAY_LEFT_TEXT_LENGTH);
    if (isAnalog) {
        this->_printEmptyBar(DISPLAY_PROGRESS_BAR_LENGTH);
    } else {
        this->_printProgressBar(DISPLAY_PROGRESS_BAR_LENGTH, value);
    }
    #endif

    // print RIGHT_TEXT
    //this->_lcd->setCursor(0, LCD_WIDTH - DISPLAY_RIGHT_TEXT_LENGTH);
    this->_lcd->print(Core::getPinMode(this->_pin)==INPUT ? DISPLAY_MODE_INPUT : DISPLAY_MODE_OUTPUT);

    if (value < 1000) {
        this->_lcd->print(' ');
    }
    if (value < 100) {
        this->_lcd->print(' ');
    }
    if (value < 10) {
        this->_lcd->print(' ');
    }

    this->_lcd->print(value);
}


void InterfaceLcd1602::_outputExtra()
{
    #if MODE_VERBOSE & MODE_VERBOSE_LIST
    uint8_t pin = NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS;

    for (uint8_t row=1; row<LCD_HEIGHT; row++) {
        this->_lcd->setCursor(0, row);

        for (uint8_t col=0; col<LCD_WIDTH; col++) {
            if (pin-->0) {
                if (Core::isPinVisible(pin)) {
                    this->_printProgressBar(1, Core::get(pin));
                }
            } else {
                this->_printEmptyBar(LCD_WIDTH - col);

                pin = 1;
                break;
            }
        }
    }
    #endif
}


void InterfaceLcd1602::_reset()
{
    this->_pin = 0;

    if (!Core::isPinVisible(this->_pin)) {
        this->_next();
    }
}


void InterfaceLcd1602::_next()
{
    do;
    while (++this->_pin < NUM_DIGITAL_PINS && !Core::isPinVisible(this->_pin));

    if (this->_pin >= NUM_DIGITAL_PINS ) {
        this->_reset();
    }
}


void InterfaceLcd1602::_switchValue()
{
    const uint8_t mode = Core::getPinMode(this->_pin);

    if (OUTPUT == mode) {
        Core::set(this->_pin, Core::get(this->_pin) < 128 ? HIGH : LOW);
    }
}


void InterfaceLcd1602::_changeValue(const int8_t delta)
{
    const uint8_t mode = Core::getPinMode(this->_pin);

    if (OUTPUT == mode) {
        Core::set(this->_pin, Core::get(this->_pin) + delta);
    }
}


void InterfaceLcd1602::_updateBacklight()
{
    switch (this->_state) {
        case SLEEPING:
            #ifdef LCD_PIN_BL
            analogWrite(LCD_PIN_BL, LCD_BACKLIGHT_SLEEP);
            #endif
            break;
        case AWAKE:
            this->_lcd->display();
            #ifdef LCD_PIN_BL
            analogWrite(LCD_PIN_BL, HIGH);
            #endif
            break;
        case IDLE:
            break;
        default:
            #ifdef LCD_PIN_BL
            analogWrite(LCD_PIN_BL, LOW);
            #endif
            this->_lcd->display();
    }
}


void InterfaceLcd1602::_printEmptyBar(uint8_t size)
{
    while (size-->0) {
        this->_lcd->write(' ');
    }
}


void InterfaceLcd1602::_printProgressBar(const uint8_t size, const uint8_t value)
{
    const uint8_t step = 256/size;
    const uint8_t halfStep = step >> 1;

    uint8_t progress = halfStep;

    for (uint8_t i=0; i<size; i++) {
        if (progress < value) {
            this->_lcd->write(DISPLAY_PB_FULL);
        } else {
            this->_lcd->write(DISPLAY_PB_VOID);
        }
        
        progress+= step;
    }
}


const boolean InterfaceLcd1602::_hasNewPulsedKey()
{
    Key currentKey = this->_getKey();

    if (this->_key != currentKey) {
        if (this->_tmpKey != currentKey) {
            this->_tmpKey = currentKey;
            
            return true;
        }
    }

    this->_key = currentKey;

    return false;
}


const InterfaceLcd1602::Key InterfaceLcd1602::_getKey()
{
    int input = analogRead(LCD_PIN_ABTN);

    if (KEYPAD_MAXVALUE_SELECT - KEYPAD_THRESHOLD < input && input < KEYPAD_MAXVALUE_SELECT + KEYPAD_THRESHOLD) return KEYPAD_SELECT;
    if (KEYPAD_MAXVALUE_LEFT   - KEYPAD_THRESHOLD < input && input < KEYPAD_MAXVALUE_LEFT   + KEYPAD_THRESHOLD) return KEYPAD_LEFT;
    if (KEYPAD_MAXVALUE_DOWN   - KEYPAD_THRESHOLD < input && input < KEYPAD_MAXVALUE_DOWN   + KEYPAD_THRESHOLD) return KEYPAD_DOWN;
    if (KEYPAD_MAXVALUE_UP     - KEYPAD_THRESHOLD < input && input < KEYPAD_MAXVALUE_UP     + KEYPAD_THRESHOLD) return KEYPAD_UP;
    if (KEYPAD_MAXVALUE_RIGHT  - KEYPAD_THRESHOLD < input && input < KEYPAD_MAXVALUE_RIGHT  + KEYPAD_THRESHOLD) return KEYPAD_RIGHT;

    return KEYPAD_NONE ;
}
