#ifndef _switch_buttons_H_
#define _switch_buttons_H_

#include <Arduino.h>

/**
 * @brief Represents a two button switch
 *
 * The constructor defines two pins that are conected to hardware buttons. One is the on button and one is the off button. When pressed short the button switches teh light on or off. When pressed long, the brighntess will increase or decrease.
 *
 * @param onPin the GPIO pin that is connected to the on button
 * @param offPin the GPIO pin that is connected to the off button
 * 
 **/

class switchButtons
{
private:
    /* data */
public:
    switchButtons(uint8_t onPin, uint8_t offpin);
    ~switchButtons();
    bool onPressed();
    bool onPressedLong();
    bool offPressed();
    bool offPressedLong();

};


#endif
